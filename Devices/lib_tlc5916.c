/*
 * Standard Library Includes
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/*
 * Device Includes
 */

#include "lib_shiftregister_common.h" 
#include "lib_shiftregister.h"
#include "lib_tlc5916.h"

/* 
 * Common and Generics Includes
 */

#include "util_macros.h"

/* Defines and typedefs */

#define LOW_CURRENT_GAIN_MINIMUM (8UL)
#define LOW_CURRENT_GAIN_MAXIMUM (33UL)

#define HIGH_CURRENT_GAIN_MINIMUM (25UL)
#define HIGH_CURRENT_GAIN_MAXIMUM (99UL)

#define LOW_CURRENT_HC_THRESHOLD (17UL)
#define HIGH_CURRENT_HC_THRESHOLD (50UL)

#define GAIN_OFFSET_HC0_CM0 (LOW_CURRENT_GAIN_MINIMUM)
#define GAIN_OFFSET_HC1_CM0 (LOW_CURRENT_HC_THRESHOLD)
#define GAIN_OFFSET_HC0_CM1 (HIGH_CURRENT_GAIN_MINIMUM)
#define GAIN_OFFSET_HC1_CM1 (HIGH_CURRENT_HC_THRESHOLD)

#define GAIN_BIT_WIDTH (6)
#define GAIN_RANGE ((1 << GAIN_BIT_WIDTH) - 1)
#define VG_SHIFT (2)
#define HC_BIT (1 << 1)
#define CM_BIT (1 << 0)

/*
 * Private Variables
 */

static const uint8_t s_percent_gain_range[4] = {
	GAIN_OFFSET_HC1_CM0 - LOW_CURRENT_GAIN_MINIMUM,
	LOW_CURRENT_GAIN_MAXIMUM - GAIN_OFFSET_HC1_CM0,
	GAIN_OFFSET_HC1_CM1 - HIGH_CURRENT_GAIN_MINIMUM,
	HIGH_CURRENT_GAIN_MAXIMUM - GAIN_OFFSET_HC1_CM1
};

static const uint8_t s_gain_offset[4] = {
	GAIN_OFFSET_HC0_CM0,
	GAIN_OFFSET_HC1_CM0,
	GAIN_OFFSET_HC0_CM1,
	GAIN_OFFSET_HC1_CM1
};

/*
 * Private Function Prototypes
 */
 
void single_clock(TLC5916_CONTROL * tlc);

void TLC5916_Init(TLC5916_CONTROL * tlc, SR_SHIFT_FN shiftOutFn, SR_IO_FN tlcLatchFn, SR_IO_FN tlcOEFn)
{
	if (tlc)
	{
		tlc->sr.shiftOutFn = shiftOutFn;
		tlc->sr.clkFn = NULL;
		tlc->sr.dataFn = NULL;
		tlc->latch = tlcLatchFn;
		tlc->oe = tlcOEFn;
	}
}

void TLC5916_ClockOut(uint8_t* data, uint8_t nBytes, TLC5916_CONTROL * tlc)
{
	tlc->sr.shiftOutFn(data, nBytes, SR_CLKEDGE_POS, SR_DIR_MSBFIRST);
	tlc->latch(true);
	tlc->latch(false);
}

void TLC5916_OutputEnable(TLC5916_CONTROL * tlc, bool enable)
{
	tlc->oe(!enable);
}

void TLC5916_SetMode(TLC5916_CONTROL * tlc, TLC5916_MODE_ENUM eMode)
{
	/* To set mode:
	 * Five clock pulses:
	Pulse 1: OE high, LE low
	Pulse 2: OE low, LE low
	Pulse 3: OE high, LE low
	Pulse 4: OE high, LE high (special mode) or low (normal mode)
	Pulse 5: OE high, LE low
	 */
	
	tlc->oe(true);
	tlc->latch(false);
	single_clock(tlc);
	
	tlc->oe(false);
	tlc->latch(false);
	single_clock(tlc);
	
	tlc->oe(true);
	tlc->latch(false);
	single_clock(tlc);
	
	tlc->oe(true);
	tlc->latch(eMode == MODE_SPECIAL);
	single_clock(tlc);
	
	tlc->oe(true);
	tlc->latch(false);
	single_clock(tlc);
}

uint8_t TLC5916_MakeCurrentGainByte(uint8_t percent_gain, bool high_current)
{
	bool set_hc_bit = false;
	
	uint32_t voltage_gain = 0;
	uint8_t final_setting = 0;
	uint8_t range_select = 0;
	
	if (high_current)
	{
		percent_gain = min(percent_gain, HIGH_CURRENT_GAIN_MAXIMUM); // Use high-current band 
		percent_gain = max(percent_gain, HIGH_CURRENT_GAIN_MINIMUM);
		set_hc_bit = (percent_gain >= HIGH_CURRENT_HC_THRESHOLD);
	}
	else
	{
		percent_gain = min(percent_gain, LOW_CURRENT_GAIN_MAXIMUM); // Use low-current band
		percent_gain = max(percent_gain, LOW_CURRENT_GAIN_MINIMUM);
		set_hc_bit = (percent_gain >= LOW_CURRENT_HC_THRESHOLD);
	}
	
	voltage_gain = 0;
	
	/* Get the array for accessing gain setting */
	range_select = (high_current ? 2 : 0) + (set_hc_bit ? 1 : 0);
	
	/* Scale and offset percent to gain for this range */
	percent_gain -= s_gain_offset[range_select];
	voltage_gain = (GAIN_RANGE * percent_gain);
	voltage_gain /= s_percent_gain_range[range_select];
	
	/* Make the bitfield */
	final_setting = voltage_gain << VG_SHIFT;
	final_setting |= (set_hc_bit ? HC_BIT : 0);
	final_setting |= (high_current ? CM_BIT : 0);
	
	return final_setting;
}

/*
 * Private Function Definitions
 */

void single_clock(TLC5916_CONTROL * tlc)
{
	tlc->sr.clkFn(false);
	tlc->sr.clkFn(true);
	tlc->sr.clkFn(false);
}

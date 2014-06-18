/*
 * Standard Library Includes
 */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include <stdlib.h>
#include <assert.h>

/*
 * AVR Includes (Defines and Primitives)
 */
 
#include <avr/io.h>
#include <util/atomic.h>

/*
 * AVR Library Includes
 */

#include "lib_clk.h"
#include "lib_io.h"
#include "lib_tmr16_pwm.h"
#include "lib_tmr16.h"

/*
 * Utility Library Includes
 */

#include "util_macros.h"

/*
 * Public Function Defintions
 */

/* TMR16_PWM_Set
 :Sets up PWM with specified frequency and duty
*/
#ifdef TMR16_PWM_DEBUG
bool TMR16_PWM_Set(uint16_t freq, uint16_t duty, TMR_OCCHAN_ENUM eChannel, TMR16_PWM_DEBUG * pData)
#else
bool TMR16_PWM_Set(uint16_t freq, uint16_t duty, TMR_OCCHAN_ENUM eChannel)
#endif
{
	// Valid TMR16 prescalers
	uint16_t prescalers[4] = {1, 8, 64, 256};
	uint8_t prescalerIndex = 0;
	uint32_t fcpu = CLK_GetFcpu();	// CPU output frequency (may be prescaled to ftmr)
	uint32_t ftmr = 0; // Timer incoming clock frequency
	uint32_t fovf = 0; // Timer overflow frequency
	uint16_t top = 0; // Sets PWM frequency
	uint16_t ocr = 0; // Sets PWM duty
	
	//Start at highest timer frequency (prescaler = 1)
    ftmr = fcpu / prescalers[prescalerIndex];
    fovf = ftmr / 65535;
	
	//Search for correct prescaler
    while (freq < fovf && prescalerIndex < 4)
	{
		prescalerIndex += 1;
        ftmr = fcpu / prescalers[prescalerIndex];
        fovf = ftmr / 65535;
    }
	
	if (prescalerIndex == 4) { return false; } // Could not resolve this frequency
	
    //Find top setting for this frequency (will be set to ICR1)
    top = div_round_pos(ftmr, freq);
	ocr = ((uint32_t)top * (uint32_t)duty) / 100UL;
	
	// This function needs to use FASTPWM_1 mode, which will set ICR1 as TOP
	// The PWM will be output on the selected channel
	TMR16_SetCountMode(TMR16_COUNTMODE_FASTPWM_1);
	TMR16_SetOutputCompareMode(TMR_OUTPUTMODE_CLEAR, eChannel);
	TMR16_SetOutputCompareValue(ocr, eChannel);
	TMR16_SetInputCapture(top);
	
	#ifdef TMR16_PWM_DEBUG
	if (pData)
	{
		pData->prescaler = prescalers[prescalerIndex];
		pData->top = top;
	}
	#endif
	return true;
}
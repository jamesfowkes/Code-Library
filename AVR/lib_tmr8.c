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

/*
 * AVR Library Includes
 */

#include "lib_tmr8.h"

/*
 * Defines and Typedefs
 */
#ifdef WGM02
#define EXTRA_WGM_MODES
#endif

#ifdef OCR0B
#define SECOND_OCR_CHANNEL
#endif

#if defined(OCR0A) || defined(TEST_HARNESS_OCR0A)
#define OCR OCR0A
#elif defined(OCR0) || defined(TEST_HARNESS_OCR0)
#define OCR OCR0
#endif

#if defined(TCCR0) || defined(TEST_HARNESS_TCCR0)
#define TCCRA TCCR0
#define TCCRB TCCR0
#elif defined(TCCR0B) || defined(TEST_HARNESS_TCCR0B)
#define TCCRA TCCR0A
#define TCCRB TCCR0B
#endif

#if defined(TIMSK0) || defined (TEST_HARNESS_TIMSK0)
#define TIMSK TIMSK0
#endif

#if !defined(FOC0A)
#define FOC0A FOC0
#endif

#if !defined(COM0A0)
#define COM0A0 COM00
#endif

#if !defined(COM0A1)
#define COM0A1 COM01
#endif

/*
 * Private Variables
 */

// Map mode number to mode enumeration
static TMR8_COUNTMODE_ENUM countModeMap[] =
{
	TMR8_COUNTMODE_NORMAL,
	TMR8_COUNTMODE_PCPWM1, 		// TOP = 0xFF
	TMR8_COUNTMODE_CTC,
	TMR8_COUNTMODE_FASTPWM1, 	// TOP = 0xFF
	#ifdef EXTRA_WGM_MODES
	TMR8_COUNTMODE_INVALID,
	TMR8_COUNTMODE_PCPWM2,		// TOP = OCR0A
	TMR8_COUNTMODE_INVALID,
	TMR8_COUNTMODE_FASTPWM2,	// TOP = OCR0A
	#endif
};

/*
 * Private Function Prototypes
 */

static bool IsPWMMode(const TMR8_COUNTMODE_ENUM eCountMode);

/*
 * Public Functions
 */

void TMR8_SetSource(TMR_SRC_ENUM eSource)
{
	uint8_t temp = 0;
	temp = TCCRB;
	temp &= ~((1 << CS02) | (1 << CS01) | (1 << CS00));
	temp |= (uint8_t)eSource;
	TCCRB = temp;
}

TMR_SRC_ENUM TMR8_GetSource(void)
{
	uint8_t temp = TCCRB;
	temp &= ((1 << CS02) | (1 << CS01) | (1 << CS00));
	return (TMR_SRC_ENUM)temp;
}

uint16_t TMR8_GetDivisor(void)
{
	TMR_SRC_ENUM temp = TMR8_GetSource();
	//TMR_SRC_FCLK,	TMR_SRC_FCLK_8,	TMR_SRC_FCLK_64, TMR_SRC_FCLK_256, TMR_SRC_FCLK_1024, TMR_SRC_FCLK_EXT_FALLING, TMR_SRC_FCLK_EXT_RISING,
	uint16_t divisors[] = {1, 1, 8, 64, 256, 1024, 1, 1};

	return divisors[temp];
}

void TMR8_SetCountMode(const TMR8_COUNTMODE_ENUM eMode)
{

	assert(eMode < TMR8_COUNTMODE_INVALID);

	// Mode is controlled by b1:0 in TCCRn and b3 in TCCRnB, if present

	if (eMode != TMR8_GetCountMode())
	{
		uint8_t tccr = TCCRA;
		tccr &= ~((1 << WGM01) | (1 << WGM00));
		
		#ifdef EXTRA_WGM_MODES
		uint8_t tccrb = TCCRB;
		tccrb &= ~((1 << WGM02));
		#endif

		switch (eMode)
		{
		case TMR8_COUNTMODE_NORMAL: // Mode 0
			// Clear all bits - nothing to do
			break;
		case TMR8_COUNTMODE_PCPWM1: // Mode 1
			tccr |= (1 << WGM00);
			break;
		case TMR8_COUNTMODE_CTC: // Mode 2
			tccr |= (1 << WGM01);
			break;
		case TMR8_COUNTMODE_FASTPWM1: // Mode 3
			tccr |= ((1 << WGM01) | (1 << WGM00));
			break;
			// Mode 4 reserved
		#ifdef EXTRA_WGM_MODES
		case TMR8_COUNTMODE_PCPWM2: // Mode 5
			tccrb |= (1 << WGM02);
			tccr |= (1 << WGM00);
			break;
			// Mode 6 reserved
		case TMR8_COUNTMODE_FASTPWM2: // Mode 7
			tccrb |= (1 << WGM02);
			tccr |= ((1 << WGM01) | (1 << WGM00));
			break;
		#else
		case TMR8_COUNTMODE_PCPWM2: // Mode 5 invalid if WGM02 not present
		case TMR8_COUNTMODE_FASTPWM2: // Mode 7 invalid if WGM02 not present
		#endif
		case TMR8_COUNTMODE_INVALID:
			assert(false);
		}

		TCCRA = tccr;
		#ifdef EXTRA_WGM_MODES
		TCCRB = tccrb;
		#endif
	}
}

TMR8_COUNTMODE_ENUM TMR8_GetCountMode(void)
{
	// Mode is controlled by b1:0 in TCCR and b3 in TCCR0B (if present)
	uint8_t tccr = TCCRA;
	uint8_t modeNumber = 0;

	#ifdef EXTRA_WGM_MODES
	uint8_t tccrb = TCCRB;
	if (tccrb & (1 << WGM02)) { modeNumber += 4; }
	#endif
	
	if (tccr & (1 << WGM01)) { modeNumber += 2; }
	if (tccr & (1 << WGM00)) { modeNumber += 1; }

	TMR8_COUNTMODE_ENUM eMode = countModeMap[modeNumber];

	assert(eMode < TMR8_COUNTMODE_INVALID);

	return eMode;
}

void TMR8_SetOutputCompareValue(const uint8_t value, const TMR_OCCHAN_ENUM eChannel)
{

	assert(eChannel < TMR_OCCHAN_INVALID);

	switch (eChannel)
	{
	case TMR_OCCHAN_A:
		OCR = value;
		break;
	case TMR_OCCHAN_B:
		#ifdef SECOND_OCR_CHANNEL
		OCR0B = value;
		#endif
		break;
	default:
		break;
	}
}

void TMR8_SetOutputCompareMode(const TMR_OUTPUTMODE_ENUM eOutputMode, const TMR_OCCHAN_ENUM eChannel)
{
	assert(eChannel < TMR_OCCHAN_INVALID);
	assert(eOutputMode < TMR_OUTPUTMODE_INVALID);

	TMR8_COUNTMODE_ENUM eCountMode = TMR8_GetCountMode();

	bool bValid = true;

	uint8_t tccr = TCCRA;

	// Map output mode selections to register values
	uint8_t newValuesA[] = { 0, (1 << COM0A0), (1 << COM0A1), (1 << COM0A0) | (1 << COM0A1) };
	#ifdef SECOND_OCR_CHANNEL
	uint8_t newValuesB[] = { 0, (1 << COM0B0), (1 << COM0B1), (1 << COM0B0) | (1 << COM0B1) };
	#endif
	
	// Check for reserved register/mode combinations
	if ((TMR_OCCHAN_B == eChannel) && (eOutputMode == TMR_OUTPUTMODE_TOGGLE))
	{
		// Toggle not allowed for PWM on channel B
		if ((TMR8_COUNTMODE_PCPWM1 == eCountMode) ||
			(TMR8_COUNTMODE_PCPWM2 == eCountMode) ||
			(TMR8_COUNTMODE_FASTPWM1 == eCountMode)	||
			(TMR8_COUNTMODE_FASTPWM1 == eCountMode)) {
			bValid = false;
		}
	}

	assert(bValid);

	if (bValid)
	{
		switch (eChannel)
		{
		case TMR_OCCHAN_A:
			tccr &= ~((1 << COM0A1) | (1 << COM0A0));
			tccr |= newValuesA[eOutputMode];
			break;
		case TMR_OCCHAN_B:
			#ifdef SECOND_OCR_CHANNEL
			tccr &= ~((1 << COM0B1) | (1 << COM0B0));
			tccr |= newValuesB[eOutputMode];
			#endif
			break;
		default:
			break;
		}
	}

	TCCRA = tccr;
}

void TMR8_ForceOutputCompare(const TMR_OCCHAN_ENUM eChannel)
{
	#ifdef TCCRB
	assert(eChannel <= TMR_OCCHAN_B);

	uint8_t tccrb = TCCRB;

	// Check that mode is non-PWM
	if ( !IsPWMMode( TMR8_GetCountMode() ) )
	{
		switch(eChannel)
		{
		case TMR_OCCHAN_A:
			tccrb |= (1 << FOC0A);
			break;
		case TMR_OCCHAN_B:
			#ifdef SECOND_OCR_CHANNEL
			tccrb |= (1 << FOC0B);
			#endif
			break;
		default:
			break;
		}
	}
	else
	{
		// Ensure both FOC bits cleared in PWM modes
		#ifdef SECOND_OCR_CHANNEL
		tccrb &= ~((1<< FOC0A) | (1 << FOC0B));
		#else
		tccrb &= ~(1<< FOC0A);
		#endif
	}

	TCCRB = tccrb;
	#else
	(void)eChannel;
	#endif
}

void TMR8_InterruptControl(TMR8_INTMASK_ENUM eMask, bool enable)
{
	uint8_t timsk = TIMSK;

	if (enable)
	{
		timsk |= (uint8_t)eMask;
	}
	else
	{
		timsk &= ~((uint8_t)eMask);
	}

	TIMSK = timsk;
}

/*
 * Private Functions
 */

static bool IsPWMMode(const TMR8_COUNTMODE_ENUM eCountMode)
{
	assert(eCountMode < TMR8_COUNTMODE_INVALID);

    return !((TMR8_COUNTMODE_NORMAL == eCountMode) || (TMR8_COUNTMODE_CTC == eCountMode));
}

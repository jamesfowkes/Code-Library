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

#include "lib_tmr16.h"

/*
 * Private Variables
 */

static TMR16_COUNTMODE_ENUM currentCountMode = TMR16_COUNTMODE_INVALID;

/*
 * Private Function Prototypes
 */

static bool IsPWMMode(const TMR16_COUNTMODE_ENUM eCountMode);

/*
 * Public Functions
 */

void TMR16_SetSource(TMR_SRC_ENUM eSource)
{
	uint8_t temp = 0;
	temp = TCCR1B;
	temp &= ~((1 << CS12) | (1 << CS11) | (1 << CS10));
	temp |= (uint8_t)eSource;
	TCCR1B = temp;
}

TMR_SRC_ENUM TMR16_GetSource(void)
{
	uint8_t temp = TCCR1B;
	temp &= ((1 << CS12) | (1 << CS11) | (1 << CS10));
	return (TMR_SRC_ENUM)temp;
}

uint16_t TMR16_GetDivisor(void)
{
	TMR_SRC_ENUM temp = TMR16_GetSource();
	//TMR_SRC_FCLK,	TMR_SRC_FCLK_8,	TMR_SRC_FCLK_64, TMR_SRC_FCLK_256, TMR_SRC_FCLK_1024, TMR_SRC_FCLK_EXT_FALLING, TMR_SRC_FCLK_EXT_RISING,
	uint16_t divisors[] = {1, 1, 8, 64, 256, 1024, 1, 1};

	return divisors[temp];
}

void TMR16_SetCountMode(const TMR16_COUNTMODE_ENUM eMode)
{

	assert(eMode < TMR16_COUNTMODE_INVALID);

	// Mode is controlled by b1:0 in TCCR1A and b4:3 in TCCR1B

	if (currentCountMode != eMode)
	{
		uint8_t tccr1a = TCCR1A;
		uint8_t tccr1b = TCCR1B;

		tccr1a &= ~((1 << WGM11) | (1 << WGM10));
		tccr1b &= ~((1 << WGM13) | (1 << WGM12));

		//Lower two bits of mode are WGM11:WGM10
		tccr1a |= (((uint8_t)eMode & 0x03) << WGM10);
		tccr1b |= ((((uint8_t)eMode & 0x04) >> 2) << WGM12);

		TCCR1A = tccr1a;
		TCCR1B = tccr1b;
	}
}

void TMR16_SetOutputCompareValue(const uint16_t value, const TMR_OCCHAN_ENUM eChannel)
{

	assert(eChannel < TMR_OCCHAN_INVALID);

	switch (eChannel)
	{
	case TMR_OCCHAN_A:
		OCR1A = value;
		break;
	case TMR_OCCHAN_B:
		OCR1B = value;
		break;
	default:
		break;
	}
}

void TMR16_SetOutputCompareMode(const TMR_OUTPUTMODE_ENUM eOutputMode, const TMR_OCCHAN_ENUM eChannel)
{
	assert(eChannel < TMR_OCCHAN_INVALID);
	assert(eOutputMode < TMR_OUTPUTMODE_INVALID);

	uint8_t tccr1a = TCCR1A;

	// Map output mode selections to register values
	uint8_t newValuesA[] = { 0, (1 << COM1A0), (1 << COM1A1), (1 << COM1A0) | (1 << COM1A1) };
	uint8_t newValuesB[] = { 0, (1 << COM1B0), (1 << COM1B1), (1 << COM1B0) | (1 << COM1B1) };

	switch (eChannel)
	{
	case TMR_OCCHAN_A:
		tccr1a &= ~((1 << COM1A1) | (1 << COM1A0));
		tccr1a |= newValuesA[eOutputMode];
		break;
	case TMR_OCCHAN_B:
		tccr1a &= ~((1 << COM1B1) | (1 << COM1B0));
		tccr1a |= newValuesB[eOutputMode];
		break;
	default:
		break;
	}
	TCCR1A = tccr1a;
}

void TMR16_ForceOutputCompare(const TMR_OCCHAN_ENUM eChannel)
{
	assert(eChannel <= TMR_OCCHAN_B);

	uint8_t tccr1c = TCCR1C;

	// Check that mode is non-PWM
	if ( !IsPWMMode( currentCountMode ) )
	{
		switch(eChannel)
		{
		case TMR_OCCHAN_A:
			tccr1c |= (1 << FOC1A);
			break;
		case TMR_OCCHAN_B:
			tccr1c |= (1 << FOC1B);
			break;
		default:
			break;
		}
	}
	else
	{
		// Ensure both FOC bits cleared in PWM modes
		tccr1c &= ~((1<< FOC1A) | (1 << FOC1B));
	}

	TCCR1C = tccr1c;
}

void TMR16_InterruptControl(TMR16_INTMASK_ENUM eMask, bool enable)
{

	uint8_t timsk1 = TIMSK1;

	if (enable)
	{
		timsk1 |= (uint8_t)eMask;
	}
	else
	{
		timsk1 &= ~((uint8_t)eMask);
	}

	TIMSK1 = timsk1;

}

/*
 * Private Functions
 */

static bool IsPWMMode(const TMR16_COUNTMODE_ENUM eCountMode)
{
	assert(eCountMode < TMR16_COUNTMODE_INVALID);

    return !((TMR16_COUNTMODE_NORMAL == eCountMode) || (TMR16_COUNTMODE_CTC == eCountMode));
}

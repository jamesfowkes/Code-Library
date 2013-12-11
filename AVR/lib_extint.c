/*
 * Standard Library Includes
 */

#include <stdbool.h>

/*
 * Defines and Typedefs
 */

#define EXTINT_ISR(name) ISR(name ## _vect) { s_bTriggered[e ## name] = true;}

/*
 * AVR Includes (Defines and Primitives)
 */
#include <avr/io.h>
#include <avr/interrupt.h>

/*
 * AVR Library Includes
 */
 
#include "lib_extint.h"

/*
 * Utility Library Includes
 */

#include "util_macros.h"

/*
 * Local Defines
 */

#if defined(GIMSK)
#define enableExternalInterrupts(mask)	(GIMSK |= (mask))
#define disableExternalInterrupts(mask)	(GIMSK &= (~(mask)))
#endif

#if defined(EIMSK)
#define enableExternalInterrupts(mask)	(EIMSK |= (mask))
#define	disableExternalInterrupts(mask)	(EIMSK &= (~(mask)))
#endif

#ifndef enableExternalInterrupts
#error "enableExternalInterrupts was not defined!"
#endif

#if defined(MCUCR)
#define SENSITIVITY_REG MCUCR
#elif defined (EICRA)
#define SENSITIVITY_REG EICRA
#endif

#define INT0_BASEBIT (1 << INT0)

static volatile bool s_bTriggered[eEXTINT_MAX];

void EXTINT_SetSensitivity(EXTINT_VECTOR_ENUM eVector, EXTINT_SENSE_ENUM sensitivity)
{
	uint8_t bitmask = ((uint8_t)sensitivity) & 0x02;
	
	bitmask <<= ((uint8_t)eVector * 2);
	
	SENSITIVITY_REG &= ~((1 << ISC01) | (1 << ISC00));
	SENSITIVITY_REG |= bitmask << ISC00;
}

void EXTINT_EnableInterrupt(EXTINT_VECTOR_ENUM eVector, bool enable)
{
	if (enable)
	{
		enableExternalInterrupts(INT0_BASEBIT << eVector);
	}
	else
	{
		disableExternalInterrupts(INT0_BASEBIT << eVector);
	}
}

bool EXTINT_TestAndClear(EXTINT_VECTOR_ENUM eVector)
{
	cli();
	bool triggered = s_bTriggered[eVector];
	s_bTriggered[eVector] = false;
	sei();
	return triggered;
}

#if defined( INT0_vect ) && !defined( SUPPRESS_EXTINT0 )
ISR(INT0_vect)
{
	s_bTriggered[eEXTINT0] = true;
}
#endif
#if defined( INT1_vect ) && !defined( SUPPRESS_EXTINT1 )
ISR(INT1_vect)
{
	s_bTriggered[eEXTINT1] = true;
}
#endif

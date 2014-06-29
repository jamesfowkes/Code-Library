/*
 * Standard Library Includes
 */

#include <stdbool.h>
#include <stdint.h>

/*
 * Defines and Typedefs
 */

#define PCINT_ISR(name) ISR(name ## _vect) { s_bTriggered[e ## name] = true;}

/*
 * AVR Includes (Defines and Primitives)
 */
#include <avr/io.h>
#include <avr/interrupt.h>

/*
 * AVR Library Includes
 */
 
#include "lib_pcint.h"

/*
 * Utility Library Includes
 */

#include "util_macros.h"

static volatile bool s_bTriggered[ePCINT_MAX];

PCINT_VECTOR_ENUM PCINT_EnableInterrupt(uint8_t pin, bool enable)
{
	// Split pin into register and pin
	// e.g PCINT10:
	// 10 / 8 = 1 => PCINT1
	// 10 - (1 * 8) = 2 => Bit 2 in PCINT1
	
	uint8_t reg = pin >> 3;
	pin -= (reg << 3);

	switch (reg)
	{
	#ifdef PCMSK0
	case 0:
		enable ? set(PCMSK0, pin) : clr(PCMSK0, pin);
		#ifdef PCICR
		PCMSK0 ? set(PCICR, PCIE0) : clr(PCICR, PCIE0);
		#endif
		#ifdef GIMSK
		enable ? set(GIMSK, PCIE0) : clr(GIMSK, PCIE0); 
		#endif
		break;
	#endif
	#ifdef PCMSK1
	case 1:
		enable ? set(PCMSK1, pin) : clr(PCMSK1, pin);
		#ifdef PCICR
		PCMSK1 ? set(PCICR, PCIE1) : clr(PCICR, PCIE1);
		#endif
		#ifdef GIMSK
		enable ? set(GIMSK, PCIE1) : clr(GIMSK, PCIE1); 
		#endif
		break;
	#endif
	#ifdef PCMSK2
	case 2:
		enable ? set(PCMSK2, pin) : clr(PCMSK2, pin);
		#ifdef PCICR
		PCMSK2 ? set(PCICR, PCIE2) : clr(PCICR, PCIE2);
		#endif
		#ifdef GIMSK
		enable ? set(GIMSK, PCIE2) : clr(GIMSK, PCIE2); 
		#endif
		break;
	#endif
	#ifdef PCMSK3
	case 3:
		enable ? set(PCMSK3, pin) : clr(PCMSK3, pin);
		#ifdef PCICR
		PCMSK3 ? set(PCICR, PCIE3) : clr(PCICR, PCIE3);
		#endif
		#ifdef GIMSK
		enable ? set(GIMSK, PCIE3) : clr(GIMSK, PCIE3); 
		#endif
		break;
	#endif
	default:
		break;
	}
	
	return (PCINT_VECTOR_ENUM)reg;
}

bool PCINT_TestAndClear(PCINT_VECTOR_ENUM eVector)
{
	cli();
	bool triggered = s_bTriggered[eVector];
	s_bTriggered[eVector] = false;
	sei();
	return triggered;
}

#if defined( PCINT0_vect ) && !defined( SUPPRESS_PCINT0 )
PCINT_ISR(PCINT0);
#endif
#if defined( PCINT1_vect ) && !defined( SUPPRESS_PCINT1 )
PCINT_ISR(PCINT1);
#endif
#if defined( PCINT2_vect ) && !defined( SUPPRESS_PCINT2 )
PCINT_ISR(PCINT2);
#endif
#if defined( PCINT3_vect ) && !defined( SUPPRESS_PCINT3 )
PCINT_ISR(PCINT3);
#endif

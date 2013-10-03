/*
 * Standard Library Includes
 */

#include <stdbool.h>

/*
 * Defines and Typedefs
 */

#define PCINT_ISR(name) ISR(name ## _vect) { s_bTriggered[e ## name] = true; }

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

static bool s_bTriggered[ePCINT_MAX];

void PCINT_EnableInterrupt(uint8_t pin, bool enable)
{
	// Split pin into register and pin
	// e.g PCINT10:
	// 10 / 8 = 1 => PCINT1
	// 10 - (1 * 8) = 2 => Bit 2 in PCINT1
	
	uint8_t reg = pin / 8;
	pin -= (reg * 8);
	
	switch (reg)
	{
	#ifdef PCMSK0
	case 0:
		enable ? set(PCMSK0, pin) : clr(PCMSK0, pin);
		break;
	#endif
	#ifdef PCMSK1
	case 1:
		enable ? set(PCMSK1, pin) : clr(PCMSK1, pin);
		break;
	#endif
	#ifdef PCMSK2
	case 2:
		enable ? set(PCMSK2, pin) : clr(PCMSK2, pin);
		break;
	#endif
	#ifdef PCMSK3
	case 3:
		enable ? set(PCMSK3, pin) : clr(PCMSK3, pin);
		break;
	#endif
	default:
		break;
	}
}

bool PCINT_TestAndClear(PCINT_VECTOR_ENUM eVector)
{
	cli();
	bool triggered = s_bTriggered[eVector];
	s_bTriggered[eVector] = false;
	sei();
	return triggered;
}

#ifdef PCINT0_vect
PCINT_ISR(PCINT0);
#endif
#ifdef PCINT1_vect
PCINT_ISR(PCINT1);
#endif
#ifdef PCINT2_vect
PCINT_ISR(PCINT2);
#endif
#ifdef PCINT3_vect
PCINT_ISR(PCINT3);
#endif

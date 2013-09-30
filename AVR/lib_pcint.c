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

static bool s_bTriggered[ePCINT_MAX];

bool PCINT_TestAndClear(PCINT_ENUM eInterrupt)
{
	cli();
	bool complete = s_bTriggered[eInterrupt];
	s_bTriggered[eInterrupt] = false;
	sei();
	return complete;
}

#ifdef INT0_vect
PCINT_ISR(INT0);
#endif
#ifdef INT1_vect
PCINT_ISR(INT1);
#endif
#ifdef INT2_vect
PCINT_ISR(INT2);
#endif
#ifdef INT3_vect
PCINT_ISR(INT3);
#endif
#ifdef INT4_vect
PCINT_ISR(INT4);
#endif
#ifdef INT5_vect
PCINT_ISR(INT5);
#endif
#ifdef INT6_vect
PCINT_ISR(INT6);
#endif
#ifdef INT7_vect
PCINT_ISR(INT7);
#endif

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

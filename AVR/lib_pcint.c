/*
 * Standard Library Includes
 */

#include <stdbool.h>

/*
 * AVR Includes (Defines and Primitives)
 */
#include <avr/io.h>
#include <avr/interrupt.h>

/*
 * AVR Library Includes
 */
 
#include "lib_pcint.h"

bool PCINT_TestAndClear(PCINT_FLAG * flag)
{
	cli();
	bool complete = true; // TODO: Get actual interrupt value
	*flag = false;
	sei();
	return complete;
}

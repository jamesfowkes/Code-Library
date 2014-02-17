/*
 * Standard Library Includes
 */
#include <stdbool.h>
#include <stddef.h>

/*
 * AVR Includes (Defines and Primitives)
 */

#include "avr/io.h"

/*
 * AVR Library Includes
 */
 
#include "lib_wdt.h"

static WDT_SLEEP_TICK * s_currentTick;

bool WDT_TestAndClear(WDT_SLEEP_TICK * tick)
{
	cli();
	bool triggered = s_currentTick.triggered
	s_currentTick.triggered = false;
	sei();
	return triggered;
}

ISR(WDT_vect)
{
	s_currentTick.triggered = true;
}
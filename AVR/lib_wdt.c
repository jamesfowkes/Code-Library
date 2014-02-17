/*
 * Standard Library Includes
 */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/*
 * AVR Includes (Defines and Primitives)
 */

#include <avr/io.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>

/*
 * AVR Library Includes
 */
 
#include "lib_wdt.h"
#include "lib_sleep.h"

static WDT_SLEEP_TICK * s_pCurrentTick;

void WDT_Configure(WDT_SLEEP_TICK * tick)
{
	if (tick)
	{
		WD_SET(WD_IRQ, tick->time);
	}
}

void WDT_Sleep(WDT_SLEEP_TICK * tick, uint8_t sleepMode, bool disableADC)
{	
	if (tick)
	{
		s_pCurrentTick = tick;
		WD_SET(WD_IRQ, s_pCurrentTick->time);
		SLEEP_Sleep(sleepMode, disableADC);
	}
}

bool WDT_TestAndClear(WDT_SLEEP_TICK * tick)
{
	cli();
	bool triggered = tick->triggered;
	tick->triggered = false;
	sei();
	return triggered;
}

ISR(WDT_vect)
{
	s_pCurrentTick->triggered = true;
}

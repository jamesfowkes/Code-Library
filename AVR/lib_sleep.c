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
#include <avr/power.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>

/*
 * AVR Library Includes
 */
 
#include "lib_sleep.h"

void SLEEP_Sleep(uint8_t sleepMode, bool disableADC)
{
	if (disableADC) { ADCSRA &= ~ADEN; }
	
	set_sleep_mode(sleepMode);
	sleep_enable();
	
	sleep_mode();
	
	sleep_disable();
	
	if (disableADC) { ADCSRA |= ADEN; }
}

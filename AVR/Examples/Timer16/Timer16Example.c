/*
 * app.c
 *
 *  Application file for stroboscope
 *
 *  Created on: 20 June 2014
 *      Author: james
 *
 *		Target: ATMEGA328P
 */

/*
 * Standard Library Includes
 */

#include <stdbool.h>
#include <string.h>
#include <stdint.h>

/*
 * Utility Includes
 */

#include "util_macros.h"

/*
 * AVR Includes (Defines and Primitives)
 */

#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/power.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>

/*
 * Device Includes
 */

// None

/*
 * Generic Library Includes
 */

// None

/*
 * AVR Library Includes
 */

#include "lib_clk.h"
#include "lib_tmr8_tick.h"
#include "lib_io.h"
#include "lib_tmr16.h"
#include "lib_tmr16_pwm.h"

/*
 * Local Application Includes
 */

// None

/*
 * Private Defines and Datatypes
 */

#define BLINK_TICK_MS 50
 
/*
 * Function Prototypes
 */

static void setupTimer(void);
static void setupIO(void);

/*
 * Private Variables
 */

static TMR8_TICK_CONFIG heartbeatTick;

int main(void)
{

	/* Disable watchdog: not required for this application */
	MCUSR &= ~(1 << WDRF);
	wdt_disable();

	setupTimer();
	setupIO();
	
	uint8_t duty = 1;
	TMR16_PWM_Set(300, duty, TMR_OCCHAN_A);

	/* All processing interrupt based from here*/

	sei();

	while (true)
	{
		if (TMR8_Tick_TestAndClear(&heartbeatTick))
		{
			IO_Control(IO_PORTB, 5, IO_TOGGLE);
			if (!TMR16_PWM_Set(300, duty++, TMR_OCCHAN_A))
			if (duty == 100) { duty = 0; }
		}
	}

	return 0;
}

/*
 * Private Functions
 */
static void setupIO(void)
{
	IO_SetMode(IO_PORTB, 1, IO_MODE_OUTPUT);
	IO_SetMode(IO_PORTB, 2, IO_MODE_OUTPUT);
	IO_SetMode(IO_PORTB, 5, IO_MODE_OUTPUT);
}

static void setupTimer(void)
{
	CLK_Init(0);
	TMR8_Tick_Init(1, 0);
	
	heartbeatTick.reload = BLINK_TICK_MS;
	heartbeatTick.active = true;
	TMR8_Tick_AddTimerConfig(&heartbeatTick);
}


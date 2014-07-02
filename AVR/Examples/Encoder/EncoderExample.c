/*
 * EncoderExample.c
 *
 *  Example AVR encoder application
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
#include "lib_encoder.h"

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

static int n;

int main(void)
{

	/* Disable watchdog: not required for this application */
	MCUSR &= ~(1 << WDRF);
	wdt_disable();

	setupTimer();
	setupIO();
	
	/* All processing interrupt based from here*/

	sei();

	while (true)
	{
		if (TMR8_Tick_TestAndClear(&heartbeatTick))
		{
			int move = ENC_GetMovement();

			n += move;

			if (n < 0) { n = 0; }
			if (n > 7) { n = 7; }

			for (int i = 0; i < move; ++i)
			{
				IO_Control(IO_PORTB, 5, IO_TOGGLE);
			}
		}
	}

	return 0;
}

/*
 * Private Functions
 */
static void setupIO(void)
{

	IO_SetMode(IO_PORTB, 5, IO_MODE_OUTPUT);

	IO_SetMode(IO_PORTD, 2, IO_MODE_INPUT);
	IO_SetMode(IO_PORTD, 3, IO_MODE_INPUT);

	ENC_Setup(IO_PORTD, 2, 3, 18, 19);
}

static void setupTimer(void)
{
	CLK_Init(0);
	TMR8_Tick_Init(1, 0);
	
	heartbeatTick.reload = BLINK_TICK_MS;
	heartbeatTick.active = true;
	TMR8_Tick_AddTimerConfig(&heartbeatTick);
}


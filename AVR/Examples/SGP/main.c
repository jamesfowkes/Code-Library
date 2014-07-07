/*
 * main.c
 *
 *  Example AVR SGP application
 *
 *  Created on: 23 June 2014
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
#include <stdio.h>
#include <math.h>

/*
 * Utility Includes
 */

// None

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

#include "lcd/lcd.h"

/*
 * Generic Library Includes
 */

// None

/*
 * AVR Library Includes
 */

#include "lib_clk.h"
#include "lib_io.h"
#include "lib_tmr8_tick.h"

/*
 * Local Application Includes
 */

#include "sgp.h"

/*
 * Private Defines and Datatypes
 */

#define BLINK_TICK_MS 500

/*
 * Function Prototypes
 */

static void setupTimer(void);

/*
 * Private Variables
 */

static TMR8_TICK_CONFIG heartbeatTick;

static char line0[] = "0 ISS (ZARYA)";
static char line1[] = "1 25544U 98067A   14135.79915963  .00006959  00000-0  12068-3 0  2984";
static char line2[] = "2 25544 051.6467 260.1457 0003007 358.3551 001.7946 15.50235663886304";

struct tle_ascii sat_data = {
	{line0,
	line1,
	line2}
};

inline void clrscr (void)
{
	register int i;
	for (i=1;(i<25);i++) printf("\n");
}

void sgp_test (void)
{
}

int main(void)
{

	/* Disable watchdog: not required for this application */
	wdt_disable();

	setupTimer();
	
	/* All processing interrupt based from here*/

	sei();

	while (true)
	{
		if (TMR8_Tick_TestAndClear(&heartbeatTick))
		{
			sgp_test();
		}
	}

	return 0;
}


static void setupTimer(void)
{
	CLK_Init(0);
	TMR8_Tick_Init(1, 0);
	
	heartbeatTick.reload = BLINK_TICK_MS;
	heartbeatTick.active = true;
	TMR8_Tick_AddTimerConfig(&heartbeatTick);
}


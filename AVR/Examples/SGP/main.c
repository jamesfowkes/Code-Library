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
	struct vector pos[5], vel[5];
	struct sgp_data sgpdata;

	int satnumber=0, interval, j;
	double delta, tsince;

	delta = 360.0;
	for (j=0;(j<5);j++)
	{
		int model=0;
		printf("\n\n\n");
		switch (j)
		{
		case 0 : 
			model = _SGP0;
			satnumber = 0;
			break;
		case 1 :
			model = _SGP4;
			satnumber = 0;
			break;
		case 2 :
			model = _SDP4;
			satnumber = 1;
			break;
		case 3 :
			model = _SGP8;
			satnumber = 0;
			break;
		case 4 :
			model = _SDP8;
			satnumber = 1;
			break;
		}

		Convert_Satellite_Data(sat_data, &sgpdata);
		for (interval=0;(interval<=4);interval++)
		{
			tsince = interval * delta;
			#if defined(SGP_MODEL_SGP0)
				sgp0(tsince,&pos[interval],&vel[interval],&sgpdata);
			#elif defined(SGP_MODEL_SGP4)
				sgp4(tsince,&pos[interval],&vel[interval],&sgpdata);
			#elif defined(SGP_MODEL_SDP4)
				sdp4(tsince,&pos[interval],&vel[interval],&sgpdata);
			#elif defined(SGP_MODEL_SGP8)
				sgp8(tsince,&pos[interval],&vel[interval],&sgpdata);
			#elif defined(SGP_MODEL_SGP0)		
				sdp8(tsince,&pos[interval],&vel[interval],&sgpdata);
			#else
				#error "No SGP model was specified!"
			#endif
			Convert_Sat_State(&pos[interval],&vel[interval]);
			pos[interval].v[3]=tsince;
		}
	}
}

int main(void)
{

	/* Disable watchdog: not required for this application */
	MCUSR &= ~(1 << WDRF);
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






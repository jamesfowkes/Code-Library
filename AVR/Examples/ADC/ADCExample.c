/*
 * ADCExample.c
 *
 *  Example AVR ADC application
 *
 *  Created on: 06 July 2014
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

//none

/*
 * AVR Library Includes
 */

#include "lib_clk.h"
#include "lib_io.h"
#include "lib_tmr8_tick.h"
#include "lib_adc.h"

/*
 * Local Application Includes
 */

// None

/*
 * Private Defines and Datatypes
 */

#define BLINK_TICK_MS 500

/*
 * Function Prototypes
 */

static void setupTimer(void);
static void setupIO(void);

/*
 * Private Variables
 */

static TMR8_TICK_CONFIG heartbeatTick;
static ADC_CONTROL_ENUM adc;

int main(void)
{

	/* Disable watchdog: not required for this application */
	MCUSR &= ~(1 << WDRF);
	wdt_disable();

	setupTimer();
	setupIO();
	
	adc.channel = LIB_ADC_CH_0;

	ADC_SelectReference(LIB_ADC_REF_VCC);
	ADC_SelectPrescaler(LIB_ADC_PRESCALER_DIV2);
	ADC_Enable(true);
	ADC_EnableInterrupts(true);

	/* All processing interrupt based from here*/

	sei();

	while (true)
	{
		if (TMR8_Tick_TestAndClear(&heartbeatTick))
		{
			IO_Control(IO_PORTB, 5, IO_TOGGLE);
			ADC_GetReading(&adc);
		}

		if (ADC_TestAndClear(&adc))
		{
			uint16_t reading = adc.reading;
			(void)reading;
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
}

static void setupTimer(void)
{
	CLK_Init(0);
	TMR8_Tick_Init(1, 0);
	
	heartbeatTick.reload = BLINK_TICK_MS;
	heartbeatTick.active = true;
	TMR8_Tick_AddTimerConfig(&heartbeatTick);
}


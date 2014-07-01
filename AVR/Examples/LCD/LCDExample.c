/*
 * LCDExample.c
 *
 *  Example AVR LCD application
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

#include "format.h"

/*
 * AVR Library Includes
 */

#include "lib_clk.h"
#include "lib_io.h"
#include "lib_tmr8_tick.h"

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

static LCD_PORT lcdDataPorts;
static LCD_PORT lcdDirectionPorts;
static LCD_PIN lcdPins;
static LCD_CONFIG lcdConfig;

static char count = 120;

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
			count++;
			lcd_gotoxy(0,0);
			lcd_puts("LCD Example");

			char buf[10];
			lcd_gotoxy(0,1);
			S8ToString(count, buf);
			lcd_puts(buf);

			IO_Control(IO_PORTB, 5, IO_TOGGLE);
		}
	}

	return 0;
}

/*
 * Private Functions
 */
static void setupIO(void)
{
	lcdDataPorts.port0.w = IO_GetWritePortDirect(IO_PORTD);
	lcdDataPorts.port1.w = IO_GetWritePortDirect(IO_PORTD);
	lcdDataPorts.port2.w = IO_GetWritePortDirect(IO_PORTD);
	lcdDataPorts.port3.w = IO_GetWritePortDirect(IO_PORTD);
	lcdDataPorts.port0.r = IO_GetReadPortDirect(IO_PORTD);
	lcdDataPorts.port1.r = IO_GetReadPortDirect(IO_PORTD);
	lcdDataPorts.port2.r = IO_GetReadPortDirect(IO_PORTD);
	lcdDataPorts.port3.r = IO_GetReadPortDirect(IO_PORTD);

	lcdDataPorts.rsPort = IO_GetWritePortDirect(IO_PORTB);
	lcdDataPorts.rwPort = IO_GetWritePortDirect(IO_PORTB);
	lcdDataPorts.enPort = IO_GetWritePortDirect(IO_PORTB);
	
	lcdDirectionPorts.port0.w = IO_GetDirectionPortDirect(IO_PORTD);
	lcdDirectionPorts.port1.w = IO_GetDirectionPortDirect(IO_PORTD);
	lcdDirectionPorts.port2.w = IO_GetDirectionPortDirect(IO_PORTD);
	lcdDirectionPorts.port3.w = IO_GetDirectionPortDirect(IO_PORTD);
	lcdDirectionPorts.rsPort = IO_GetDirectionPortDirect(IO_PORTB);
	lcdDirectionPorts.rwPort = IO_GetDirectionPortDirect(IO_PORTB);
	lcdDirectionPorts.enPort = IO_GetDirectionPortDirect(IO_PORTB);
	
	lcdPins.pin0 = 2;
	lcdPins.pin1 = 3;
	lcdPins.pin2 = 4;
	lcdPins.pin3 = 5;
	lcdPins.rsPin = 0;
	lcdPins.rwPin = 1;
	lcdPins.enPin = 2;
	
	lcdConfig.type = LCD_CONTROLLER_HD44780;
	lcdConfig.lines = 2;
	lcdConfig.dispLength = 16;
	lcdConfig.internalLength = 0x40;
	lcdConfig.line1Start = 0x00;
	lcdConfig.line2Start = 0x40;
	lcdConfig.line3Start = 0x14;
	lcdConfig.line4Start = 0x54;
	lcdConfig.wrapLines = false;

	lcd_init(LCD_DISP_ON, &lcdDataPorts, &lcdDirectionPorts, &lcdPins, &lcdConfig);

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


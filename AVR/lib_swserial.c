/*
 * Standard Library Includes
 */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#ifdef PC_TEST_HARNESS
#include <stdio.h>
#include "avr_compatibility.h"
#endif

/*
 * Defines
 */

/*
 * AVR Includes (Defines and Primitives)
 */
#ifndef PC_TEST_HARNESS
#include <avr/io.h>
#include <util/delay.h>
#include <util/atomic.h>
#include <avr/interrupt.h>
#endif

/*
 * AVR Library Include
 */
#ifndef PC_TEST_HARNESS
#include "lib_io.h"
#else
#include "lib_io_serial_test.h"
#endif

#include "lib_swserial.h"

/*
 * Private Defines and Typedefs
 */

/*
 * Private Variables
 */

static IO_PORT_ENUM s_ePort;
static uint8_t s_pin;
static LIB_SWS_BAUDRATE_ENUM s_eBaudrate;

static char txBuffer[128];

/*
 * Private Function Prototypes
 */

static void txDelay(void);

static void translateBuffer(char const * const buffer, uint8_t size, void * args[], uint8_t nargs);
static uint8_t format(char * buf, const char spec, void * pArg);
static uint8_t getPlaceCount(uint16_t value, uint16_t place);

/*
 * Public Functions
 */

void SWS_Init(IO_PORT_ENUM ePort, uint8_t pin, LIB_SWS_BAUDRATE_ENUM eBaudrate)
{
	s_ePort = ePort;
	s_pin = pin;
	
	s_eBaudrate = eBaudrate;
	
	IO_SetMode(s_ePort, s_pin, IO_MODE_OUTPUT);
	IO_Control(s_ePort, s_pin, IO_ON);
}

void SWS_SetBaudrate(LIB_SWS_BAUDRATE_ENUM eBaudrate)
{
	s_eBaudrate = eBaudrate;
}

void SWS_Transmit(char const * const buffer, uint8_t size, void * args[], uint8_t nargs)
{
	if (nargs > 0 && args)
	{
		translateBuffer(buffer, size, args, nargs);
	}
	else
	{
		strcpy(txBuffer, buffer);
	}


	uint8_t mask = 0x00;
	uint8_t i = 0;

	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		while (txBuffer[i])
		{
			mask = 0x01;
			IO_Control(s_ePort, s_pin, IO_OFF);
			txDelay(); // Start bit

			for (mask = 0x01; mask > 0; mask <<= 1)
			{
				IO_Control(s_ePort, s_pin, (txBuffer[i] & mask) ? IO_ON : IO_OFF);
				txDelay();
			}

			IO_Control(s_ePort, s_pin, IO_ON);
			txDelay(); // Stop bit
			i++;
		}
	}

}

/*
 * Private Functions
 */
static void txDelay(void)
{
	uint16_t usdelay = 0;

	usdelay = 3200;

	while (usdelay)
	{
		if (usdelay > 700)
		{
			_delay_us(700);
			usdelay -= 700;
		}
		else
		{
			_delay_us(usdelay);
			usdelay = 0;
		}
	}
}

static void translateBuffer(char const * const buffer, uint8_t size, void * args[], uint8_t nargs)
{
	uint8_t i = 0;	//Index into user buffer
	uint8_t tx = 0;	//Index into transmit buffer
	uint8_t arg = 0;	//Index into argument array

	if (size == 0)
	{
		size = strlen((char*)buffer);
	}

	if (size)
	{
		do
		{
			if ((buffer[i] == '%') && (i+1 < size) && (nargs > 0))
			{
				tx += format(&txBuffer[tx], buffer[i+1], args[arg++]);
				++i; // Skip formatting character
			}
			else
			{
				txBuffer[tx++] = buffer[i];
			}
		} while (++i < size);
	}
	txBuffer[tx++] = '\r';
	txBuffer[tx++] = '\n';
	txBuffer[tx] = '\0';
}

static uint8_t format(char * buf, const char spec, void * pArg)
{
	uint8_t bytesWritten = 0;

	uint16_t uVal = 0;
	int16_t sVal = 0;
	uint8_t digitcount = 0;
	bool foundSignificantDigit = false;

	switch(spec)
	{
	case 'u':// Unsigned 8-bit int
		uVal = *( (uint16_t *)pArg );
		break;
	case 'U':// Unsigned 16-bit int
		uVal = (uint16_t)( *( (uint16_t *)pArg ) );
		break;
	case 's':// Signed 8-bit int
		sVal = (int16_t)( *( (int8_t *)pArg ) );
		uVal = abs(sVal);
		break;
	case 'S':// Signed 16-bit int
		sVal = (int16_t)( *( (int16_t *)pArg ) );
		uVal = abs(sVal);
		break;
	}

	if (sVal < 0)
	{
		*buf++ = '-';
		bytesWritten++;
	}

	uint16_t placevalue = 0;

	for (placevalue = 10000; placevalue > 1; placevalue /= 10)
	{
		digitcount = getPlaceCount(uVal, placevalue);
		if (digitcount || foundSignificantDigit)
		{
			foundSignificantDigit = true;
			*buf++ = (char)(digitcount + '0');
			uVal -= (digitcount * placevalue);
			bytesWritten++;
		}
	}

	digitcount = uVal;
	*buf++ = (char)(digitcount + '0');
	bytesWritten++;

	return bytesWritten;
}

static uint8_t getPlaceCount(uint16_t value, uint16_t place)
{

	if (place > 1 && place > 0)
	{
		return (uint8_t)(value / place);
	}
	else
	{
		return value - (10 * (value / 10));
	}
}

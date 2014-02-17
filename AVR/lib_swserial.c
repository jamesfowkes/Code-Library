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
#include <util/delay_basic.h>
#include <util/atomic.h>
#include <avr/interrupt.h>
#endif

/*
 * AVR Library Include
 */
#ifndef PC_TEST_HARNESS
#include "lib_io.h"
#include "lib_fuses.h"
#else
#include "lib_io_serial_test.h"
#endif

#include "lib_swserial.h"

/*
 * Utility Library Includes
 */

#include "util_macros.h"

/*
 * Private Defines and Typedefs
 */

enum
{
	TX,
	RX
};

#define START_CONDITION (true)

#define delay() _delay_loop_2(s_DelayCount)

#define US_TO_DELAYCOUNTS(us)	div_round(F_CPU * (us), 4000000 + s_uCPUDiv)

/*
 * Private Variables
 */

static volatile uint8_t * s_pPort[2];
static uint8_t s_pin[2];

static char txBuffer[TX_BUFFER_SIZE];

static uint16_t s_DelayCount = 0;

static uint8_t s_uCPUDiv = 0;

/*
 * Private Function Prototypes
 */

static void translateBuffer(char const * const buffer, uint8_t size, void * args[], uint8_t nargs);
static uint8_t format(char * buf, const char spec, void * pArg);
static uint8_t getPlaceCount(uint32_t value, uint32_t place);
static void transmit(void);

/*
 * Public Functions
 */

void SWS_SetBaudRate(LIB_SWS_BAUDRATE_ENUM eBaudrate)
{
	s_uCPUDiv = FUS_IsClockDiv8Enabled() ? 8 : 0;
	s_DelayCount = US_TO_DELAYCOUNTS( (uint16_t)div_round(1000000UL, (uint32_t)eBaudrate) );
}

void SWS_RxInit(IO_PORT_ENUM ePort, uint8_t pin)
{
	s_pPort[RX] = IO_GetPortDirect(ePort);
	s_pin[RX] = pin;
	
	IO_On(*s_pPort[RX], s_pin[RX]);
}

void SWS_TxInit(IO_PORT_ENUM ePort, uint8_t pin)
{
	s_pPort[TX] = IO_GetPortDirect(ePort);
	s_pin[TX] = pin;
	IO_SetMode(ePort, pin, IO_MODE_OUTPUT);
	IO_On(*s_pPort[TX], s_pin[TX]);
}

uint8_t SWS_Receive(char * rxBuffer, uint8_t n, bool breakOnNull)
{
	/* This function will be called when an start condition interrupt is 
	detected. Since timing from the first edge cannot be relied upon, wait
	for a second start condition */
	
	while (IO_Read(*s_pPort[RX], s_pin[RX]) == START_CONDITION)
	{
		// Wait for first start condition to disappear
	}
	
	while (IO_Read(*s_pPort[RX], s_pin[RX]) != START_CONDITION)
	{
		// Wait for second start condition to appear
	}
	
	uint8_t mask = 0x00;
	uint8_t i = 0;
	
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		while (i < n)
		{
			mask = 0x01;
			delay(); // Start bit
			for (mask = 0x01; mask > 0; mask <<= 1)
			{

				rxBuffer[i] |= (IO_Read(*s_pPort[RX], s_pin[RX]) ? mask : 0);
				delay();
			}
			delay(); // Stop bit
			
			if (breakOnNull && (rxBuffer[i] == '\0')) { break; }

			i++;
		}
	}
	
	return i;
}
	
void SWS_SimpleTransmit(char const * const buffer)
{
	strcpy(txBuffer, buffer);
	transmit();
}

/* Valid argument formats: %u/%U/%L: 8/16/32-bit unsigned, %s/%S: 8/16-bit signed */
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
	
	transmit();
}

static void transmit(void)
{

	uint8_t mask = 0x00;
	uint8_t i = 0;

	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		while (txBuffer[i])
		{
			mask = 0x01;
			IO_Off(*s_pPort[TX], s_pin[TX]);
			delay(); // Start bit

			for (mask = 0x01; mask > 0; mask <<= 1)
			{

				(txBuffer[i] & mask) ? IO_On(*s_pPort[TX], s_pin[TX]) : IO_Off(*s_pPort[TX], s_pin[TX]);
				delay();
			}

			IO_On(*s_pPort[TX], s_pin[TX]);
			delay(); // Stop bit
			i++;
		}
	}
}

/*
 * Private Functions
 */

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

	uint32_t uVal = 0;
	int32_t sVal = 0;
	uint8_t digitcount = 0;
	bool foundSignificantDigit = false;

	switch(spec)
	{
	case 'u':// Unsigned 8-bit int
		uVal = (uint32_t)( *( (uint8_t *)pArg ) );
		break;
	case 'U':// Unsigned 16-bit int
		uVal = (uint32_t)( *( (uint16_t *)pArg ) );
		break;
	case 's':// Signed 8-bit int
		sVal = (int32_t) ( *( (int8_t *)pArg ) );
		uVal = labs(sVal);
		break;
	case 'S':// Signed 16-bit int
		sVal = (int32_t) ( *( (int16_t *)pArg ) );
		uVal = labs(sVal);
		break;
	case 'L':// Signed 32-bit int
		uVal = (uint32_t)( *( (uint32_t *)pArg ) );
		break;
	}

	if (sVal < 0)
	{
		*buf++ = '-';
		bytesWritten++;
	}

	uint32_t placevalue = 0;

	for (placevalue = 1000000000; placevalue > 1; placevalue /= 10)
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

static uint8_t getPlaceCount(uint32_t value, uint32_t place)
{
	if (place > 1 && value > 0)
	{
		return (uint8_t)(value / place);
	}
	else
	{
		return value - (10 * (value / 10));
	}
}

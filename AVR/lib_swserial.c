/*
 * Standard Library Includes
 */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

/*
 * Defines
 */

#ifdef TEST_HARNESS
#define F_CPU 8000000
#endif

/*
 * AVR Includes (Defines and Primitives)
 */
#include <avr/io.h>
#include <util/delay_basic.h>
#include <util/atomic.h>
#include <avr/interrupt.h>

/*
 * AVR Library Include
 */

#include "lib_io.h"
#include "lib_fuses.h"

#include "lib_swserial.h"

/*
 * Format Library Includes
 */

#include "Format/format.h"

/*
 * Utility Library Includes
 */

#include "utility/util_macros.h"

/*
 * Private Defines and Typedefs
 */

enum
{
	TX,
	RX
};

#define START_CONDITION (true)

#define delay() _delay_loop_2(s_DelayCount - 6)

#define US_TO_DELAYCOUNTS(us)	div_round(F_CPU * (us), 4000000 * s_uCPUDiv)

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
	s_pPort[RX] = IO_GetReadPortDirect(ePort);
	s_pin[RX] = pin;
	
	IO_On(*s_pPort[RX], s_pin[RX]);
}

void SWS_TxInit(IO_PORT_ENUM ePort, uint8_t pin)
{
	s_pPort[TX] = IO_GetWritePortDirect(ePort);
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
		TranslateBuffer(buffer, txBuffer, size, args, nargs, true);
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

#ifdef TEST_HARNESS
char * SWS_GetTxBuffer(void) { return txBuffer; }
#endif

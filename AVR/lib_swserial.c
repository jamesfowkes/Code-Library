/*
 * Standard Library Includes
 */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

/*
 * Defines
 */

 /*
 * AVR Includes (Defines and Primitives)
 */
 
#include <avr/io.h>
#include <util/delay.h>

/*
 * AVR Library Include
 */

#include "lib_io.h"
#include "lib_swserial.h"

/*
 * Private Defines and Typedefs
 */

enum BitDelayFraction
{
	POINT33,
	POINT66
};
typedef enum BitDelayFraction BITDELAYFRACTION;

/*
 * Private Variables
 */
 
static IO_PORT_ENUM s_ePort;
static uint8_t s_pin;
static LIB_SWS_BAUDRATE_ENUM s_eBaudrate;
/*
 * Private Function Prototypes
 */

static void txDelay(void);
static bool getDelayForAverage(uint16_t *pUsDelay, BITDELAYFRACTION delayFraction, uint8_t bitCount);

/*
 * Public Functions
 */

void SWS_Init(IO_PORT_ENUM ePort, uint8_t pin, LIB_SWS_BAUDRATE_ENUM eBaudrate)
{
	s_ePort = ePort;
	s_pin = pin;
	
	s_eBaudrate = eBaudrate;
	
	IO_SetMode(s_ePort, s_pin, IO_MODE_OUTPUT);

}

void SWS_SetBaudrate(LIB_SWS_BAUDRATE_ENUM eBaudrate)
{
	s_eBaudrate = eBaudrate;
}

void SWS_Transmit(uint8_t const * const buffer, uint8_t size)
{
	uint8_t mask = 0x80;
	
	if (size)
	{
		while(size--)
		{
			mask = 0x80;
			while (mask)
			{
				IO_Control(s_ePort, s_pin, (buffer[size] & mask) ? IO_ON : IO_OFF);
				mask >>= 1;
				txDelay();
			}
		}

		IO_Control(s_ePort, s_pin, IO_OFF);
	}
}

/*
 * Private Functions
 */
static void txDelay(void)
{
	uint16_t usdelay = 0;

	static uint32_t bitCount = 0;
	bool bResetCount = false;
	
	/* Integer delays in microseconds are hardcoded.
	 * In order to achieve greater precision, some bits
	 * (depending on baud rate) are lengthened to achieve
	 * a 0.33 or 0.66 extra microseconds delay on average.
	 */
	switch (s_eBaudrate)
	{
	case LIB_SWS_BAUD_300:
		usdelay = 3333;
		bResetCount = getDelayForAverage(&usdelay, POINT33, bitCount);
		break;
	case LIB_SWS_BAUD_600:
		usdelay = 1666;
		bResetCount = getDelayForAverage(&usdelay, POINT66, bitCount);
		break;
	case LIB_SWS_BAUD_1200:
		usdelay = 833;
		bResetCount = getDelayForAverage(&usdelay, POINT33, bitCount);
		break;
	case LIB_SWS_BAUD_2400:
		usdelay = 416;
		bResetCount = getDelayForAverage(&usdelay, POINT66, bitCount);
		break;
	case LIB_SWS_BAUD_4800:
		usdelay = 208;
		bResetCount = getDelayForAverage(&usdelay, POINT33, bitCount);
		break;
	case LIB_SWS_BAUD_9600:
		usdelay = 104;
		bResetCount = true;
		break;
	}
	
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

	if (bResetCount)
	{
		bitCount = 0;
	}
	else
	{
		bitCount++;
	}
}

static bool getDelayForAverage(uint16_t *pUsDelay, BITDELAYFRACTION delayFraction, uint8_t bitCount)
{
	bool bResetCount = false;
	
	switch (delayFraction)
	{
	case POINT33:
		if (bitCount == 2)
		{
			// Third bit needs extending a little
			*pUsDelay += 1;
			bResetCount = true;
		}
		break;
	case POINT66:
		if (bitCount < 2)
		{
			// Bits 0 and 1 need extending a little
			*pUsDelay += 1;
			bResetCount = true;
		}
		break;
	}
	
	return bResetCount;
}

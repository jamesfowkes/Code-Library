/*
 * Standard Library Includes
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/*
 * Device Includes
 */

#include "lib_shiftregister_common.h"

/*
 * AVR Library Includes
 */

#include "lib_shiftregister.h"

static volatile uint8_t * s_clockPort;
static uint8_t s_clockPin;
static volatile uint8_t * s_dataPort;
static uint8_t s_dataPin;

void SR_Init(volatile uint8_t * clockPort, uint8_t clockPin, volatile uint8_t * dataPort, uint8_t dataPin)
{
	s_clockPort = clockPort;
	s_clockPin = clockPin;
	s_dataPort = dataPort;
	s_dataPin = dataPin;
}

void SR_ShiftOut(uint8_t* data, uint8_t nBytes, SR_CLKEDGE_ENUM edge, SR_DIRECTION_ENUM dir)
{
	uint8_t b = 0;
	uint8_t mask;
	
	for (; b < nBytes; ++b)
	{
		if (dir == SR_DIR_MSBFIRST)
		{
			for (mask = 0x80; mask; mask >>= 1)
			{
				// Clear the clock
				(edge != SR_CLKEDGE_POS) ? (*s_clockPort |= (1 << s_clockPin)) : (*s_clockPort &= ~(1 << s_clockPin));

				// Setup data
				((data[b] & mask) == mask) ? (*s_dataPort |= (1 << s_dataPin)) : (*s_dataPort &= ~(1 << s_dataPin));

				// Assert the clock
				(edge == SR_CLKEDGE_POS) ? (*s_clockPort |= (1 << s_clockPin)) : (*s_clockPort &= ~(1 << s_clockPin));
			}
		}
		else
		{
			for (mask = 0x01; mask; mask <<= 1)
			{
				// Clear the clock
				(edge != SR_CLKEDGE_POS) ? (*s_clockPort |= (1 << s_clockPin)) : (*s_clockPort &= ~(1 << s_clockPin));

				// Setup data
				((data[b] & mask) == mask) ? (*s_dataPort |= (1 << s_dataPin)) : (*s_dataPort &= ~(1 << s_dataPin));

				// Assert the clock
				(edge == SR_CLKEDGE_POS) ? (*s_clockPort |= (1 << s_clockPin)) : (*s_clockPort &= ~(1 << s_clockPin));
			}
		}
	}
}

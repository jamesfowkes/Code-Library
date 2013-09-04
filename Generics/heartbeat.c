/*
 * Standard Library Includes
 */

#include <stdbool.h>
#include <stdint.h>

/*
 * Utility Library includes
 */

#include "heartbeat.h"

/*
 * Private Variables
 */

static uint8_t s_msTick = 0;
static uint16_t s_msCount = 0;
static uint16_t s_msPeriod = 0;

/*
 * Public Functions
 */
void Heartbeat_Init(uint8_t msTick, uint16_t msBeat)
{
	s_msTick = msTick;
	s_msBeat = msBeat;
	s_msCount = 0;
}

bool Heartbeat_Tick(void)
{
	s_msCount += s_msTick;
	
	bool beat = (s_msCount >= s_msBeat);
	
	if (beat)
	{
		s_msCount = 0;
	}
	
	return beat;
}

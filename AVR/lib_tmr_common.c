/*
 * Standard Library Includes
 */

#include <stdbool.h>
#include <stdint.h>

/*
 * AVR Library Includes
 */

#include "lib_io.h"
#include "lib_tmr_common.h"


/*
 * Private Variables
 */

static IO_PORT_ENUM outputPort[2];
static uint8_t outputPin[2];

void TMR_SetOutputPorts(TMR_OCCHAN_ENUM eChannel, IO_PORT_ENUM ePort, uint8_t pin)
{
	outputPort[eChannel] = ePort;
	outputPin[eChannel] = pin;
}

/*
 * Publuc Functions
 */
void TMR_PWMOff(const TMR_OCCHAN_ENUM eChannel, const IO_STATE_ENUM eState)
{
	/* Reset channel to requested state */
	switch (eChannel)
	{
	case TMR_OCCHAN_A:
		IO_SetMode(outputPort[eChannel], outputPin[eChannel], eState);
		break;
	case TMR_OCCHAN_B:
		IO_SetMode(outputPort[eChannel], outputPin[eChannel], eState);
		break;
	default:
		break;
	}
}

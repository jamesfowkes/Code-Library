/*
 * Standard Library Includes
 */

#include <stdint.h>

/*
 * AVR Library Includes
 */

#include "lib_io.h"

void IO_Control(volatile uint8_t *port, IO_STATE_ENUM eState, uint8_t pin)
{
	switch(eState)
	{
	case IO_OFF:
		*port &= ~(1 << pin);
		break;
	case IO_ON:
		*port |= (1 << pin);
		break;
	}
}

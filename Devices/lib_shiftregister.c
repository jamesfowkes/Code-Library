/*
 * Standard Library Includes
 */

#include <stdint.h>
#include <stdbool.h>

/*
 * Device Includes
 */
 
#include "lib_shiftregister.h"

/*
 * Public Functions
 */

void SR_ShiftOut(uint8_t* data, uint8_t nBytes, SR_CONTROL * ctrl)
{
	uint8_t b;
	uint8_t mask;
	
	for (; b < nBytes; ++b)
	{
		for (mask = 0x80; mask; mask >>= 1)
		{
		
			// Clear the clock
			ctrl->clkFn(ctrl->edge != SR_CLKEDGE_POS);
			
			// Setup data
			ctrl->dataFn((data[b] & mask) == mask);
			
			// Assert the clock
			ctrl->clkFn(ctrl->edge == SR_CLKEDGE_POS);
		}
	}
}

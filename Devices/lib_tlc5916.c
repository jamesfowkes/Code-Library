/*
 * Standard Library Includes
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/*
 * Device Includes
 */
 
#include "lib_shiftregister.h"
#include "lib_tlc5916.h"

void TLC5916_ClockOut(uint8_t* data, uint8_t nBytes, TLC5916_CONTROL * tlc)
{
	ctrl->edge = SR_CLKEDGE_POS;
	
	tlc->latch(false);
	SR_ShiftOut(data, nBytes, ctrl);
	tlc->latch(true);
}

void TLC5916_OutputEnable(TLC5916_CONTROL * tlc, bool enable)
{
	tlc->oe(!enable);
}

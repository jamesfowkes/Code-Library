/*
 * Standard Library Includes
 */
#include <stdbool.h>
#include <stddef.h>

/*
 * AVR Includes (Defines and Primitives)
 */
#include "avr/io.h"
#include <util/delay.h>
/*
 * AVR Library Includes
 */
#include "lib_clk.h"
#include "lib_fuses.h"
#include "lib_io.h"

/*
 * Private Variables
 */
static uint32_t fExt = 0UL;

static clock_div_t ePrescaler = clock_div_8;
FUSE_SETTINGS * pFuses = NULL;

/*
 * Public Functions
 */
bool CLK_Init(const uint32_t fExtSet)
{
	fExt = fExtSet;
	ePrescaler = FUS_IsClockDiv8Enabled() ? clock_div_8 : clock_div_1;

	return true;
}

void CLK_SetPrescaler(const clock_div_t eSetPrescaler)
{
	ePrescaler = eSetPrescaler;
	clock_prescale_set(ePrescaler);
}

clock_div_t CLK_GetPrescaler(void)
{
	return ePrescaler;
}

bool CLK_IsSourceRunning(LIB_CLK_SRC_ENUM eSource)
{
#ifdef CKSTA
	uint8_t check = 0;

	switch (eSource)
	{
		case LIB_CLK_SRC_RC:
		check = 1 << RCON;
		break;
		case LIB_CLK_SRC_EXT:
		check = 1 << EXTON;
		break;
		default:
		break;
	}

	return (CKSTA & check) == check;
#else
	return LIB_CLK_SRC_RC == eSource;
#endif
}

uint32_t CLK_GetFcpu(void)
{
	uint32_t base = 0;

	if (CLK_IsSourceRunning(LIB_CLK_SRC_RC))
	{
		base = F_CPU;
	}
	else if (CLK_IsSourceRunning(LIB_CLK_SRC_EXT))
	{
		base = fExt;
	}

	base /= (1 << (uint8_t) ePrescaler);

	return base;
}

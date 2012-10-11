/*
 * Standard Library Includes
 */
#include <stdbool.h>

/*
 * AVR Includes (Defines and Primitives)
 */
#include "avr/io.h"
#include "avr/power.h"

/*
 * AVR Library Includes
 */
#include "lib_clk.h"

/*
 * Private Function Prototypes
 */
void SelectRcOscillator(void);
void SelectExternalOscillator(void);

/*
 * Private Variables
 */
static uint32_t fExt = 0UL;
static clock_div_t ePrescaler = clock_div_1;
static LIB_CLK_SRC_ENUM eSource = LIB_CLK_SRC_RC;

/*
 * Public Functions
 */
bool CLK_Init(const uint32_t fExtSet)
{
	fExt = fExtSet;
	return true;
}

void CLK_SetSource(const LIB_CLK_SRC_ENUM eSetSource)
{
	if (eSetSource != eSource)
	{
		switch(eSetSource)
		{
		case LIB_CLK_SRC_RC:
			SelectRcOscillator();
			break;
		case LIB_CLK_SRC_EXT:
			SelectExternalOscillator();
			break;
		default:
			break;
		}
	}
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
}

uint32_t CLK_GetFcpu(void)
{
	uint32_t base = 0;
	switch (eSource)
	{
	case LIB_CLK_SRC_RC:
		base = F_CPU;
		break;
	case LIB_CLK_SRC_EXT:
		base = fExt;
		break;
	default:
		break;
	}

	base /= (1 << (uint8_t)ePrescaler);

	return base;
}

/*
 * Private Functions
 */
void SelectRcOscillator(void)
{
	PLLCSR &= ~(1 << PLLE);							// Disable PLL
	CKSEL0 |= (1 << RCE);							// Enable RC CLock
	while ( (CKSTA & (1 << RCON)) != (1 << RCON));	// Wait for clock ready
	CKSEL0 &= ~(1 << CLKS);							// Select the RC clock
	CKSEL0 &= ~(1 << EXTE);							// Disable the external clock
}

void SelectExternalOscillator(void)
{
	CKSEL0 |= (1 << EXTE);								// Enable the external clock
	while ( (CKSTA & (1 << EXTON)) != (1 << EXTON));	// Wait for external clock ready
	CKSEL0 |= (1 << CLKS);								// Select the external clock
	PLLCSR |= (1 << PLLE);								// Enable the PLL
	CKSEL0 &= ~(1 << RCE);								// Disable the internal clock
	while ( (PLLCSR & (1 << PLOCK)) != (1 << PLOCK));	// Wait for PLL to lock
}

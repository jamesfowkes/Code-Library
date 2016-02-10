/*
 * Standard library includes
 */

#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stddef.h>
#include <math.h>

/*
 * Utility Library Includes
 */

#include "Utility/util_macros.h"

/*
 * Device Library Includes
 */

#include "lib_pot_divider.h"

/*
 * Private defines and typedefs
 */

/*
 * Private Variables
 */

/*
 * Private Functions
 */

static uint32_t getOtherResistanceWithFixedPulldown(POT_DIVIDER * pDivider, uint16_t adcReading)
{
	uint32_t resistance = 0UL;

	if (adcReading == 0)
	{
		resistance = POTDIVIDER_GetInfiniteResitance();
	}
	else
	{
		// D = U(M/A - 1) = UM/A - U -> avoid precision loss
		resistance = pDivider->rDivider * pDivider->maxAdcReading;
		resistance = div_round(resistance, adcReading);
		resistance -= pDivider->rDivider;
	}
	return resistance;
}

static uint32_t getOtherResistanceWithFixedPullup(POT_DIVIDER * pDivider, uint16_t adcReading)
{
	uint32_t resistance = pDivider->rDivider * adcReading;
	resistance = div_round(resistance, (pDivider->maxAdcReading - adcReading));
	return resistance;
}


/*
 * Public Functions
 */

bool POTDIVIDER_Init(POT_DIVIDER * pDivider, uint16_t maxAdcReading, uint32_t rDivider, DIVIDER_TYPE eDividerType)
{
	bool success = false;
	if (pDivider)
	{
		success = true;
		pDivider->maxAdcReading = maxAdcReading;
		pDivider->rDivider = rDivider;
		pDivider->eDividerType = eDividerType;
	}
	return success;
}

uint32_t POTDIVIDER_GetResistanceFromADC(POT_DIVIDER * pDivider, uint16_t adcReading)
{
	uint32_t resistance = 0UL;
	
	if (pDivider)
	{

		switch(pDivider->eDividerType)
		{
		case FIXED_PULLDOWN:
			resistance = getOtherResistanceWithFixedPulldown(pDivider, adcReading);
			break;
		case FIXED_PULLUP:
			resistance = getOtherResistanceWithFixedPullup(pDivider, adcReading);
			break;
		}
	}
	
	return resistance;
}

uint16_t POTDIVIDER_GetADCFromResistance(POT_DIVIDER * pDivider, uint32_t resistance)
{
	uint64_t adc = 0UL;
	
	if (pDivider)
	{
		switch(pDivider->eDividerType)
		{
		case FIXED_PULLDOWN:
			adc = pDivider->maxAdcReading * pDivider->rDivider;
			adc = div_round(adc, pDivider->rDivider + resistance);
			break;
		case FIXED_PULLUP:
			adc = pDivider->maxAdcReading * (uint64_t)resistance;
			adc = div_round(adc, pDivider->rDivider + resistance);
			break;
		}
	}
	
	return (uint16_t)adc;
}

uint32_t POTDIVIDER_GetInfiniteResitance()
{
	return (uint32_t)INFINITY;
}
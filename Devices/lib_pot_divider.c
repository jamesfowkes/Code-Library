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

#include "util_macros.h"

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
 * Private Function Prototypes
 */

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
		if ((adcReading > 0) && (adcReading < pDivider->maxAdcReading))
		{		
			switch(pDivider->eDividerType)
			{
			case PULLDOWN:
				resistance = pDivider->rDivider * pDivider->maxAdcReading;
				resistance = div_round(resistance, adcReading);
				resistance -= pDivider->rDivider;
				break;
			case PULLUP:
				resistance = pDivider->rDivider * adcReading;
				resistance = div_round(resistance, (1023 - adcReading));
				break;
			}
		}
	}
	
	return resistance;
}

uint16_t POTDIVIDER_GetADCFromResistance(POT_DIVIDER * pDivider, uint32_t resistance)
{
	uint32_t adc = 0UL;
	
	if (pDivider)
	{
		switch(pDivider->eDividerType)
		{
		case PULLDOWN:
			adc = pDivider->maxAdcReading * pDivider->rDivider;
			adc = div_round(adc, pDivider->rDivider + resistance);
			break;
		case PULLUP:
			adc = pDivider->maxAdcReading * resistance;
			adc = div_round(adc, pDivider->rDivider + resistance);
			break;
		}
	}
	
	return (uint16_t)adc;
}

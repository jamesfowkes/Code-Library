#ifndef _LIB_POT_DIVIDER_H_
#define _LIB_POT_DIVIDER_H_

/*
 * Defines and typedefs
 */

enum divider_type
{
	FIXED_PULLDOWN,
	FIXED_PULLUP
};
typedef enum divider_type DIVIDER_TYPE;

struct pot_divider
{
	uint16_t maxAdcReading;
	uint32_t rDivider;
	DIVIDER_TYPE eDividerType;
};
typedef struct pot_divider POT_DIVIDER;

/*
 * Public Function Prototypes
 */

bool POTDIVIDER_Init(POT_DIVIDER * pDivider, uint16_t maxAdcReading, uint32_t rDivider, DIVIDER_TYPE eDividerType);
uint32_t POTDIVIDER_GetResistanceFromADC(POT_DIVIDER * pDivider, uint16_t adcReading);
uint16_t POTDIVIDER_GetADCFromResistance(POT_DIVIDER * pDivider, uint32_t resistance);
uint32_t POTDIVIDER_GetInfiniteResitance();

#endif

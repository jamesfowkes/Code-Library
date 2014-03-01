#ifndef _LIB_THERMISTOR_H_
#define _LIB_THERMISTOR_H_

/*
 * Defines and typedefs
 */

enum divider_type
{
	PULLDOWN,
	PULLUP
};
typedef enum divider_type DIVIDER_TYPE;
struct thermistor
{
	FIXED_POINT_TYPE B;
	FIXED_POINT_TYPE Rinf;
};
typedef struct thermistor THERMISTOR;

struct thermistor_divider_reading
{
	uint16_t maxReading;
	uint32_t rDivider;
	DIVIDER_TYPE eDividerType;
};
typedef struct thermistor_divider_reading THERMISTOR_DIVIDER_READING;

/*
 * Public Function Prototypes
 */

void THERMISTOR_Init(void);
bool THERMISTOR_InitDevice(THERMISTOR * pTherm, uint16_t B, uint32_t R25);
bool THERMISTOR_InitDivider(THERMISTOR_DIVIDER_READING * pDivider, uint16_t maxReading, uint32_t rDivider, DIVIDER_TYPE eDividerType);

FIXED_POINT_TYPE THERMISTOR_GetReading(THERMISTOR * pTherm, uint32_t R);
FIXED_POINT_TYPE THERMISTOR_GetDividerReading(THERMISTOR * pTherm, THERMISTOR_DIVIDER_READING * pDivider, uint16_t reading);

uint32_t THERMISTOR_GetResistance(THERMISTOR * pTherm, FIXED_POINT_TYPE t);

#endif

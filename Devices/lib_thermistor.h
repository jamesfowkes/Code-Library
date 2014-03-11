#ifndef _LIB_THERMISTOR_H_
#define _LIB_THERMISTOR_H_

/*
 * Defines and typedefs
 */

struct thermistor
{
	FIXED_POINT_TYPE B;
	FIXED_POINT_TYPE Rinf;
};
typedef struct thermistor THERMISTOR;

/*
 * Public Function Prototypes
 */

void THERMISTOR_Init(void);
bool THERMISTOR_InitDevice(THERMISTOR * pTherm, uint16_t B, uint32_t R25);

FIXED_POINT_TYPE THERMISTOR_TemperatureFromResistance(THERMISTOR * pTherm, uint32_t R);
FIXED_POINT_TYPE THERMISTOR_TemperatureFromADCReading(THERMISTOR * pTherm, POT_DIVIDER * pDivider, uint16_t reading);

uint32_t THERMISTOR_GetResistanceFromTemperature(THERMISTOR * pTherm, FIXED_POINT_TYPE t);

#endif

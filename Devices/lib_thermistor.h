#ifndef _LIB_THERMISTOR_H_
#define _LIB_THERMISTOR_H_

/*
 * Defines and typedefs
 */

#ifdef USE_FIX16
typedef fix16_t DECIMAL_TYPE;
#else
typedef float DECIMAL_TYPE;
#endif

struct thermistor
{
	DECIMAL_TYPE B;
	DECIMAL_TYPE Rinf;
};
typedef struct thermistor THERMISTOR;

struct thermistor_divider_reading
{
	uint16_t maxReading;
	uint32_t rPulldown;
};
typedef struct thermistor_divider_reading THERMISTOR_DIVIDER_READING;

/*
 * Public Function Prototypes
 */

void THERMISTOR_Init(void);
bool THERMISTOR_InitDevice(THERMISTOR * pTherm, uint16_t B, uint32_t R25);
bool THERMISTOR_InitDivider(THERMISTOR_DIVIDER_READING * pDivider, uint16_t maxReading, uint32_t rPulldown);

DECIMAL_TYPE THERMISTOR_GetReading(THERMISTOR * pTherm, uint32_t R);
DECIMAL_TYPE THERMISTOR_GetDividerReading(THERMISTOR * pTherm, THERMISTOR_DIVIDER_READING * pDivider, uint16_t reading);

uint32_t THERMISTOR_GetResistance(THERMISTOR * pTherm, DECIMAL_TYPE t);

#endif

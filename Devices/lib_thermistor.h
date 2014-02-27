#ifndef _LIB_THERMISTOR_H_
#define _LIB_THERMISTOR_H_

/*
 * Defines and typedefs
 */

struct thermistor
{
	fix16_t B;
	fix16_t Rinf;
};
typedef struct thermistor THERMISTOR;

/*
 * Public Function Prototypes
 */

void THERMISTOR_Init(void);
bool THERMISTOR_InitDevice(THERMISTOR * pTherm, uint16_t B, uint32_t R25);
fix16_t THERMISTOR_GetReading(THERMISTOR * pTherm, uint32_t R);
uint32_t THERMISTOR_GetResistance(THERMISTOR * pTherm, fix16_t t);

#endif

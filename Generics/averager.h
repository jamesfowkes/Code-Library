#ifndef _AVERAGER_H_
#define _AVERAGER_H_

/*
 * Defines and Typedefs
 */

typedef struct averager8 AVERAGER8;
typedef struct averager16 AVERAGER16;
typedef struct averager32 AVERAGER32;

AVERAGER8 * AVERAGER8_GetAverager(uint8_t size);
AVERAGER16 * AVERAGER16_GetAverager(uint8_t size);
AVERAGER32 * AVERAGER32_GetAverager(uint8_t size);

void AVERAGER8_NewData(AVERAGER8 * pAverager, int8_t newData);
void AVERAGER16_NewData(AVERAGER16 * pAverager, int16_t newData);
void AVERAGER32_NewData(AVERAGER32 * pAverager, int32_t newData);

int8_t AVERAGER8_GetAverage(AVERAGER8 * pAverager);
int16_t AVERAGER16_GetAverage(AVERAGER16 * pAverager);
int32_t AVERAGER32_GetAverage(AVERAGER32 * pAverager);

#endif

#ifndef _AVERAGER_H_
#define _AVERAGER_H_

/*
 * Defines and Typedefs
 */

typedef struct averager8 AVERAGER8;
typedef struct averager16 AVERAGER16;
typedef struct averager32 AVERAGER32;

typedef struct averageru8 AVERAGERU8;
typedef struct averageru16 AVERAGERU16;
typedef struct averageru32 AVERAGERU32;

AVERAGER8 * AVERAGER8_GetAverager(uint8_t size);
AVERAGER16 * AVERAGER16_GetAverager(uint8_t size);
AVERAGER32 * AVERAGER32_GetAverager(uint8_t size);
AVERAGERU8 * AVERAGERU8_GetAverager(uint8_t size);
AVERAGERU16 * AVERAGERU16_GetAverager(uint8_t size);
AVERAGERU32 * AVERAGERU32_GetAverager(uint8_t size);

void AVERAGER8_NewData(AVERAGER8 * pAverager, int8_t newData);
void AVERAGER16_NewData(AVERAGER16 * pAverager, int16_t newData);
void AVERAGER32_NewData(AVERAGER32 * pAverager, int32_t newData);

void AVERAGERU8_NewData(AVERAGERU8 * pAverager, uint8_t newData);
void AVERAGERU16_NewData(AVERAGERU16 * pAverager, uint16_t newData);
void AVERAGERU32_NewData(AVERAGERU32 * pAverager, uint32_t newData);

int8_t AVERAGER8_GetAverage(AVERAGER8 * pAverager);
int16_t AVERAGER16_GetAverage(AVERAGER16 * pAverager);
int32_t AVERAGER32_GetAverage(AVERAGER32 * pAverager);

uint8_t AVERAGERU8_GetAverage(AVERAGERU8 * pAverager);
uint16_t AVERAGERU16_GetAverage(AVERAGERU16 * pAverager);
uint32_t AVERAGERU32_GetAverage(AVERAGERU32 * pAverager);

void AVERAGER8_Reset(AVERAGER8 * pAverager, int8_t value);
void AVERAGER16_Reset(AVERAGER16 * pAverager, int16_t value);
void AVERAGER32_Reset(AVERAGER32 * pAverager, int32_t value);

void AVERAGERU8_Reset(AVERAGERU8 * pAverager, uint8_t value);
void AVERAGERU16_Reset(AVERAGERU16 * pAverager, uint16_t value);
void AVERAGERU32_Reset(AVERAGERU32 * pAverager, uint32_t value);

#endif

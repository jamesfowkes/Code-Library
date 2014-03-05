#ifndef _AVERAGER_H_
#define _AVERAGER_H_

/*
 * Defines and Typedefs
 */

typedef struct averager AVERAGER;

AVERAGER * AVERAGER_GetAverager(uint8_t size);
void AVERAGER_NewData(AVERAGER * pAverager, int32_t newData);
uint32_t AVERAGER_GetAverage(AVERAGER * pAverager);

#endif

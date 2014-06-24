#ifndef _AVERAGER_H_
#define _AVERAGER_H_

/*
 * Defines and Typedefs
 */

typedef struct averager AVERAGER;

AVERAGER * AVERAGER_GetAverager(INTEGERTYPE eType, uint8_t size);

void AVERAGER_NewData(AVERAGER * pAverager, void * pNewData);

void AVERAGER_GetAverage(AVERAGER * pAverager, void * pResult);

void AVERAGER_Reset(AVERAGER * pAverager, void * pValue);

#endif

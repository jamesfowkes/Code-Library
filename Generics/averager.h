#ifndef _AVERAGER_H_
#define _AVERAGER_H_

/*
 * Defines and Typedefs
 */

enum averager_type
{
	S8,
	U8,
	S16,
	U16,
	S32,
	U32
};
typedef enum averager_type AVERAGER_TYPE;

typedef struct averager AVERAGER;

AVERAGER * AVERAGER_GetAverager(AVERAGER_TYPE eType, uint8_t size);

void AVERAGER_NewData(AVERAGER * pAverager, void * pNewData);

void AVERAGER_GetAverage(AVERAGER * pAverager, void * pResult);

void AVERAGER_Reset(AVERAGER * pAverager, void * pValue);

#endif

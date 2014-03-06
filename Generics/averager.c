/*
 * Standard Library Includes
 */
 
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

/*
 * Generic Library Includes
 */

#include "memorypool.h"
#include "ringbuf.h"
#include "averager.h"
#include "util_macros.h"

/*
 * Defines and Typedefs
 */

struct averager
{
	void * data;
	uint8_t maxIndex;
	uint8_t iWrite;
	bool full;
};
typedef struct averager AVERAGER;

struct averager8
{
	int8_t * data;
	uint8_t maxIndex;
	uint8_t iWrite;
	bool full;
};

struct averager16
{
	int16_t * data;
	uint8_t maxIndex;
	uint8_t iWrite;
	bool full;
};

struct averager32
{
	int32_t * data;
	uint8_t maxIndex;
	uint8_t iWrite;
	bool full;
};

#define RETURN_AVERAGER_PTR(averager_type, data_type, nElements) \
averager_type * pAverager = NULL; \
if (nElements) \
{ \
	pAverager = (averager_type *)MEMPOOL_GetBytes(sizeof(averager_type)); \
	if (pAverager) \
	{ \
		pAverager->data = (data_type*)MEMPOOL_GetBytes(nElements * sizeof(data_type)); \
		if (pAverager->data) \
		{ \
			pAverager->iWrite = 0; \
			pAverager->maxIndex = nElements - 1; \
			pAverager->full = false; \
		} \
		else \
		{ \
			pAverager = NULL; \
		} \
	} \
} \
return pAverager;

#define WRITE_NEW_DATA(pAverager, newData, type) \
type* pTypedPtr = (type*)pAverager; \
if (pTypedPtr) \
{ \
	pTypedPtr->data[pTypedPtr->iWrite] = newData; \
	pTypedPtr->full |= (pTypedPtr->iWrite == pTypedPtr->maxIndex); \
	incrementwithrollover(pTypedPtr->iWrite, pTypedPtr->maxIndex); \
}

#define RETURN_AVERAGE(pTypedPtr, rtntype) \
int32_t average = 0; \
uint8_t count = 0; \
if (pTypedPtr) \
{ \
	if (pTypedPtr->iWrite || pTypedPtr->full) \
	{ \
		uint8_t n = 0; \
		count = pTypedPtr->full ? pTypedPtr->maxIndex : pTypedPtr->iWrite - 1; \
		for (n = 0; n <= count; n++) \
		{ \
			average += pTypedPtr->data[n]; \
		} \
	} \
	else \
	{ \
		return 0; \
	} \
} \
else \
{ \
	return 0; \
} \
return (rtntype)div_round(average, count+1);

#define RESET_AVERAGER(pTypedPtr, value) \
if (pTypedPtr) \
{ \
	for (uint8_t n = 0; n <= pTypedPtr->maxIndex; n++) \
	{ \
		pTypedPtr->data[n] = value; \
	} \
	pTypedPtr->iWrite = 0; \
	pTypedPtr->full = (value != 0); \
}

/*
 * Private Function Prototypes
 */
 
/*
 * Private Variables
 */
 
/*
 * Public Function Definitions
 */

AVERAGER8 * AVERAGER8_GetAverager(uint8_t size)
{
	RETURN_AVERAGER_PTR(AVERAGER8, int8_t, size);
}

AVERAGER16 * AVERAGER16_GetAverager(uint8_t size)
{
	RETURN_AVERAGER_PTR(AVERAGER16, int16_t, size);
}

AVERAGER32 * AVERAGER32_GetAverager(uint8_t size)
{
	RETURN_AVERAGER_PTR(AVERAGER32, int32_t, size);
}

void AVERAGER8_NewData(AVERAGER8 * pAverager, int8_t newData)
{
	WRITE_NEW_DATA(pAverager, newData, AVERAGER8)
}

void AVERAGER16_NewData(AVERAGER16 * pAverager, int16_t newData)
{
	WRITE_NEW_DATA(pAverager, newData, AVERAGER16)
}

void AVERAGER32_NewData(AVERAGER32 * pAverager, int32_t newData)
{
	WRITE_NEW_DATA(pAverager, newData, AVERAGER32)
}

int8_t AVERAGER8_GetAverage(AVERAGER8 * pAverager)
{
	RETURN_AVERAGE(pAverager, int8_t);
}

int16_t AVERAGER16_GetAverage(AVERAGER16 * pAverager)
{
	RETURN_AVERAGE(pAverager, int16_t);
}

int32_t AVERAGER32_GetAverage(AVERAGER32 * pAverager)
{
	RETURN_AVERAGE(pAverager, int32_t);
}

void AVERAGER8_Reset(AVERAGER8 * pTypedPtr, int8_t value)
{
	RESET_AVERAGER(pTypedPtr, value);
}

void AVERAGER16_Reset(AVERAGER16 * pTypedPtr, int16_t value)
{
	RESET_AVERAGER(pTypedPtr, value);
}

void AVERAGER32_Reset(AVERAGER32 * pTypedPtr, int32_t value)
{
	RESET_AVERAGER(pTypedPtr, value);
}

/*
 * Private Function Defintions
 */

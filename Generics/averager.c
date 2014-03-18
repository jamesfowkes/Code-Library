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

#define AVERAGER_TYPE_STRUCT(averager_type, data_type) \
struct averager_type \
{ \
	data_type * data; \
	uint8_t maxIndex; \
	uint8_t iWrite; \
	bool full; \
}; \

AVERAGER_TYPE_STRUCT(averageru8, uint8_t);
AVERAGER_TYPE_STRUCT(averager8, int8_t);
AVERAGER_TYPE_STRUCT(averageru16, uint16_t);
AVERAGER_TYPE_STRUCT(averager16, int16_t);
AVERAGER_TYPE_STRUCT(averageru32, uint32_t);
AVERAGER_TYPE_STRUCT(averager32, int32_t);

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

#define GET_AVERAGER_FUNCTION(averager_type, data_type) \
averager_type * averager_type##_GetAverager(uint8_t size) \
{ \
	RETURN_AVERAGER_PTR(averager_type, data_type, size); \
}

#define WRITE_NEW_DATA(pAverager, newData, type) \
type* pTypedPtr = (type*)pAverager; \
if (pTypedPtr) \
{ \
	pTypedPtr->data[pTypedPtr->iWrite] = newData; \
	pTypedPtr->full |= (pTypedPtr->iWrite == pTypedPtr->maxIndex); \
	incrementwithrollover(pTypedPtr->iWrite, pTypedPtr->maxIndex); \
}

#define NEW_DATA_FUNCTION(averager_type, data_type) \
void averager_type##_NewData(averager_type * pAverager, data_type newData) \
{ \
	WRITE_NEW_DATA(pAverager, newData, averager_type) \
} \

#define RETURN_AVERAGE(pTypedPtr, rtntype, sumtype) \
sumtype average = 0; \
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

#define GET_SIGNED_AVERAGE_FUNCTION(averager_type, data_type) \
data_type averager_type##_GetAverage(averager_type * pAverager) \
{ \
	RETURN_AVERAGE(pAverager, data_type, int32_t); \
} \

#define GET_UNSIGNED_AVERAGE_FUNCTION(averager_type, data_type) \
data_type averager_type##_GetAverage(averager_type * pAverager) \
{ \
	RETURN_AVERAGE(pAverager, data_type, uint32_t); \
} \

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

#define RESET_FUNCTION(averager_type, data_type) \
void averager_type##_Reset(averager_type * pTypedPtr, data_type value) \
{ \
	RESET_AVERAGER(pTypedPtr, value); \
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

GET_AVERAGER_FUNCTION(AVERAGER8, int8_t)
GET_AVERAGER_FUNCTION(AVERAGERU8, uint8_t)
GET_AVERAGER_FUNCTION(AVERAGER16, int16_t)
GET_AVERAGER_FUNCTION(AVERAGERU16, uint16_t)
GET_AVERAGER_FUNCTION(AVERAGER32, int32_t)
GET_AVERAGER_FUNCTION(AVERAGERU32, uint32_t)

NEW_DATA_FUNCTION(AVERAGER8, int8_t)
NEW_DATA_FUNCTION(AVERAGERU8, uint8_t)
NEW_DATA_FUNCTION(AVERAGER16, int16_t)
NEW_DATA_FUNCTION(AVERAGERU16, uint16_t)
NEW_DATA_FUNCTION(AVERAGER32, int32_t)
NEW_DATA_FUNCTION(AVERAGERU32, uint32_t)

GET_SIGNED_AVERAGE_FUNCTION(AVERAGER8, int8_t)
GET_UNSIGNED_AVERAGE_FUNCTION(AVERAGERU8, uint8_t)
GET_SIGNED_AVERAGE_FUNCTION(AVERAGER16, int16_t)
GET_UNSIGNED_AVERAGE_FUNCTION(AVERAGERU16, uint16_t)
GET_SIGNED_AVERAGE_FUNCTION(AVERAGER32, int32_t)
GET_UNSIGNED_AVERAGE_FUNCTION(AVERAGERU32, uint32_t)

RESET_FUNCTION(AVERAGER8, int8_t)
RESET_FUNCTION(AVERAGERU8, uint8_t)
RESET_FUNCTION(AVERAGER16, int16_t)
RESET_FUNCTION(AVERAGERU16, uint16_t)
RESET_FUNCTION(AVERAGER32, int32_t)
RESET_FUNCTION(AVERAGERU32, uint32_t)

/*
 * Private Function Defintions
 */

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

 // This struct typedef'd in header file
struct averager
{
	void * data;
	AVERAGER_TYPE type;
	uint8_t maxIndex;
	uint8_t iWrite;
	bool full;
};

#define WRITE_NEW_DATA(pAverager, pNewData, data_type) \
if (pAverager) \
{ \
	((data_type*)pAverager->data)[pAverager->iWrite] = *(data_type*)pNewData; \
	pAverager->full |= (pAverager->iWrite == pAverager->maxIndex); \
	incrementwithrollover(pAverager->iWrite, pAverager->maxIndex); \
}

#define DEFINE_GET_AVERAGE_FUNCTION(type, data_type, sumtype) \
static void Get##type##Average(AVERAGER* pAverager, void* pResult) \
{ \
	sumtype average = 0; \
	uint8_t count = 0; \
	if (pAverager) \
	{ \
		if (pAverager->iWrite || pAverager->full) \
		{ \
			uint8_t n = 0; \
			count = pAverager->full ? pAverager->maxIndex : pAverager->iWrite - 1; \
			for (n = 0; n <= count; n++) \
			{ \
				average += ((data_type*)(pAverager->data))[n]; \
			} \
		} \
		else \
		{ \
			*(data_type*)pResult = 0; \
		} \
	} \
	else \
	{ \
		*(data_type*)pResult = 0; \
	} \
	*(data_type*)pResult = (data_type)div_round(average, count+1); \
}

#define DEFINE_GET_SIGNED_AVERAGE_FUNCTION(type, data_type) DEFINE_GET_AVERAGE_FUNCTION(type, data_type, int32_t);
#define DEFINE_GET_UNSIGNED_AVERAGE_FUNCTION(type, data_type) DEFINE_GET_AVERAGE_FUNCTION(type, data_type, uint32_t);

#define DEFINE_RESET_AVERAGER_FUNCTION(type, data_type) \
static void Reset##type##Data(AVERAGER* pAverager, void* pValue) \
{ \
	if (pAverager) \
	{ \
		data_type value = (pValue) ? *(data_type*)pValue : 0; \
		for (uint8_t n = 0; n <= pAverager->maxIndex; n++) \
		{ \
			((data_type*)pAverager->data)[n] = value; \
		} \
	} \
}

/*
 * Private Function Prototypes
 */
 
/*
 * Private Variables
 */

/*
 * Private Function Definitions
 */

DEFINE_GET_SIGNED_AVERAGE_FUNCTION(S8, int8_t)
DEFINE_GET_UNSIGNED_AVERAGE_FUNCTION(U8, uint8_t)
DEFINE_GET_SIGNED_AVERAGE_FUNCTION(S16, int16_t)
DEFINE_GET_UNSIGNED_AVERAGE_FUNCTION(U16, uint16_t)
DEFINE_GET_SIGNED_AVERAGE_FUNCTION(S32, int32_t)
DEFINE_GET_UNSIGNED_AVERAGE_FUNCTION(U32, uint32_t)

DEFINE_RESET_AVERAGER_FUNCTION(S8, int8_t)
DEFINE_RESET_AVERAGER_FUNCTION(U8, uint8_t)
DEFINE_RESET_AVERAGER_FUNCTION(S16, int16_t)
DEFINE_RESET_AVERAGER_FUNCTION(U16, uint16_t)
DEFINE_RESET_AVERAGER_FUNCTION(S32, int32_t)
DEFINE_RESET_AVERAGER_FUNCTION(U32, uint32_t)

static void * getDataPointer(AVERAGER_TYPE eType, uint8_t nElements)
{
	switch(eType)
	{
	case S8:
		return (void*)MEMPOOL_GetBytes(nElements * sizeof(int8_t));
	case U8:
		return (void*)MEMPOOL_GetBytes(nElements * sizeof(uint8_t));
	case S16:
		return (void*)MEMPOOL_GetBytes(nElements * sizeof(int16_t));
	case U16:
		return (void*)MEMPOOL_GetBytes(nElements * sizeof(uint16_t));
	case S32:
		return (void*)MEMPOOL_GetBytes(nElements * sizeof(int32_t));
	case U32:
		return (void*)MEMPOOL_GetBytes(nElements * sizeof(uint32_t));
	default:
		return NULL;
	}
}

/*
 * Public Function Definitions
 */

AVERAGER * AVERAGER_GetAverager(AVERAGER_TYPE eType, uint8_t size)
{
	AVERAGER* pAverager = NULL;
	
	if (size)
	{
		pAverager = (AVERAGER*)MEMPOOL_GetBytes(sizeof(AVERAGER));
		if (pAverager)
		{
			pAverager->data = getDataPointer(eType, size);
			
			if (pAverager->data)
			{
				pAverager->type = eType;
				pAverager->iWrite = 0;
				pAverager->maxIndex = size - 1;
				pAverager->full = false;
			}
			else
			{
				pAverager = NULL;
			}
		}
	}
	return pAverager;
}

void AVERAGER_NewData(AVERAGER * pAverager, void * pNewData)
{
	switch(pAverager->type)
	{
	case S8:
		WRITE_NEW_DATA(pAverager, pNewData, int8_t);
		break;
	case U8:
		WRITE_NEW_DATA(pAverager, pNewData, uint8_t);
		break;
	case S16:
		WRITE_NEW_DATA(pAverager, pNewData, int16_t);
		break;
	case U16:
		WRITE_NEW_DATA(pAverager, pNewData, uint16_t);
		break;
	case S32:
		WRITE_NEW_DATA(pAverager, pNewData, int32_t);
		break;
	case U32:
		WRITE_NEW_DATA(pAverager, pNewData, uint32_t);
		break;
	}
}

void AVERAGER_GetAverage(AVERAGER * pAverager, void * pResult)
{
	if (pAverager && pResult)
	{
		switch(pAverager->type)
		{
		case S8:
			return GetS8Average(pAverager, pResult);
		case U8:
			return GetU8Average(pAverager, pResult);
		case S16:
			return GetS16Average(pAverager, pResult);
		case U16:
			return GetU16Average(pAverager, pResult);
		case S32:
			return GetS32Average(pAverager, pResult);
		case U32:
			return GetU32Average(pAverager, pResult);
		}
	}
}

void AVERAGER_Reset(AVERAGER * pAverager, void * pValue)
{
	if (pAverager)
	{
		switch(pAverager->type)
		{
		case S8:
			ResetS8Data(pAverager, pValue);
			break;
		case U8:
			ResetU8Data(pAverager, pValue);
			break;
		case S16:
			ResetS16Data(pAverager, pValue);
			break;
		case U16:
			ResetU16Data(pAverager, pValue);
			break;
		case S32:
			ResetS32Data(pAverager, pValue);
			break;
		case U32:
			ResetU32Data(pAverager, pValue);
			break;
		}
		
		pAverager->iWrite = 0;
		/* If no pValue supplied, client wanted to fill with "NULL" zeros 
		- i.e. zeros not to be counted in future averages */
		pAverager->full = (pValue != NULL); 
	}
	
}

/*
 * Private Function Defintions
 */

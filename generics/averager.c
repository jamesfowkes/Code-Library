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

#include "integertypeenum.h" 
#include "memorypool.h"
#include "ringbuf.h"
#include "averager.h"
#include "Utility/util_macros.h"

/*
 * Defines and Typedefs
 */

 // This struct typedef'd in header file
struct averager
{
	void * data;
	INTEGERTYPE type;
	uint8_t maxIndex;
	uint8_t iWrite;
	bool full;
};

#ifdef INCLUDE_32BIT_AVERAGER
typedef int64_t SUMMING_TYPE;
#else
typedef int32_t SUMMING_TYPE;
#endif

#define DEFINE_RESET_AVERAGER_FUNCTION(type, data_type) \
static void Reset##type##Data(AVERAGER* pAverager, void* pValue) \
{ \
	data_type value = (pValue) ? *(data_type*)pValue : 0; \
	for (uint8_t n = 0; n <= pAverager->maxIndex; n++) \
	{ \
		((data_type*)pAverager->data)[n] = value; \
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

DEFINE_RESET_AVERAGER_FUNCTION(S8, int8_t)
DEFINE_RESET_AVERAGER_FUNCTION(U8, uint8_t)
DEFINE_RESET_AVERAGER_FUNCTION(S16, int16_t)
DEFINE_RESET_AVERAGER_FUNCTION(U16, uint16_t)
#ifdef INCLUDE_32BIT_AVERAGER
DEFINE_RESET_AVERAGER_FUNCTION(S32, int32_t)
DEFINE_RESET_AVERAGER_FUNCTION(U32, uint32_t)
#endif

static void * allocateDataPointer(INTEGERTYPE eType, uint8_t nElements)
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
#ifdef INCLUDE_32BIT_AVERAGER
	case S32:
		return (void*)MEMPOOL_GetBytes(nElements * sizeof(int32_t));
	case U32:
		return (void*)MEMPOOL_GetBytes(nElements * sizeof(uint32_t));
#endif
	default:
		return NULL;
	}
}

static int32_t getData(AVERAGER * pAverager, uint8_t n)
{
	switch(pAverager->type)
	{
	case S8:
		return (int32_t)(((int8_t*)pAverager->data)[n]);
	case U8:
		return (int32_t)(((uint8_t*)pAverager->data)[n]);
	case S16:
		return (int32_t)(((int16_t*)pAverager->data)[n]);
	case U16:
		return (int32_t)(((uint16_t*)pAverager->data)[n]);
#ifdef INCLUDE_32BIT_AVERAGER
	case S32:
		return (int32_t)(((int32_t*)pAverager->data)[n]);
	case U32:
		return (int32_t)(((uint32_t*)pAverager->data)[n]);
#endif
	default:
		return 0;
	}
}

static SUMMING_TYPE getAverage(AVERAGER * pAverager)
{
	SUMMING_TYPE sum = 0;
	uint8_t count = 0;
	if (pAverager)
	{
		if (pAverager->iWrite || pAverager->full)
		{
			uint8_t n = 0;
			count = pAverager->full ? pAverager->maxIndex : pAverager->iWrite - 1;
			for (n = 0; n <= count; n++)
			{
				sum += getData(pAverager, n);
			}
		}
		else
		{
			sum = 0;
		}
	}
	else
	{
		sum = 0;
	}
	sum = div_round(sum, count+1);
	return sum;
}

/*
 * Public Function Definitions
 */

AVERAGER * AVERAGER_GetAverager(INTEGERTYPE eType, uint8_t size)
{
	AVERAGER* pAverager = NULL;
	
	if (size)
	{
		pAverager = (AVERAGER*)MEMPOOL_GetBytes(sizeof(AVERAGER));
		if (pAverager)
		{
			pAverager->data = allocateDataPointer(eType, size);
			
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
	if (pAverager)
	{
		switch(pAverager->type)
		{
		case S8:
			((int8_t*)pAverager->data)[pAverager->iWrite] = *(int8_t*)pNewData;
			break;
		case U8:
			((int8_t*)pAverager->data)[pAverager->iWrite] = *(uint8_t*)pNewData;
			break;
		case S16:
			((int16_t*)pAverager->data)[pAverager->iWrite] = *(int16_t*)pNewData;
			break;
		case U16:
			((uint16_t*)pAverager->data)[pAverager->iWrite] = *(uint16_t*)pNewData;
			break;
#ifdef INCLUDE_32BIT_AVERAGER
		case S32:
			((int32_t*)pAverager->data)[pAverager->iWrite] = *(int32_t*)pNewData;
			break;
		case U32:
			((uint32_t*)pAverager->data)[pAverager->iWrite] = *(uint32_t*)pNewData;
			break;
#endif	
		}
		pAverager->full |= (pAverager->iWrite == pAverager->maxIndex);
		incrementwithrollover(pAverager->iWrite, pAverager->maxIndex);
	}
}

void AVERAGER_GetAverage(AVERAGER * pAverager, void * pResult)
{
	if (pAverager && pResult)
	{
		switch(pAverager->type)
		{
		case S8:
			*(int8_t*)pResult = (int8_t)getAverage(pAverager);
			break;
		case U8:
			*(uint8_t*)pResult = (uint8_t)getAverage(pAverager);
			break;
		case S16:
			*(int16_t*)pResult = (int16_t)getAverage(pAverager);
			break;
		case U16:
			*(uint16_t*)pResult = (uint16_t)getAverage(pAverager);
			break;
#ifdef INCLUDE_32BIT_AVERAGER			
		case S32:
			*(int32_t*)pResult = (int32_t)getAverage(pAverager);
			break;
		case U32:
			*(uint32_t*)pResult = (uint32_t)getAverage(pAverager);
			break;
#endif			
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
#ifdef INCLUDE_32BIT_AVERAGER			
		case S32:
			ResetS32Data(pAverager, pValue);
			break;
		case U32:
			ResetU32Data(pAverager, pValue);
			break;
#endif			
		}
		
		pAverager->iWrite = 0;
		/* If no pValue supplied, client wanted to fill with "NULL" zeros 
		- i.e. zeros not to be counted in future averages */
		pAverager->full = (pValue != NULL); 
	}
	
}

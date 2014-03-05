#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

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
	int32_t * data;
	uint8_t maxIndex;
	uint8_t iWrite;
	bool full;
};

/*
 * Private Variables
 */

/*
 * Public Function Definitions
 */
 
AVERAGER * AVERAGER_GetAverager(uint8_t size)
{
	 
	AVERAGER * pAverager = NULL;
	
	if (size)
	{
		pAverager = (AVERAGER *)MEMPOOL_GetBytes(sizeof(AVERAGER));
		if (pAverager)
		{
			pAverager->data = (int32_t * )MEMPOOL_GetBytes(size * sizeof(uint32_t));
			pAverager->iWrite = 0;
			pAverager->maxIndex = size - 1;
			pAverager->full = false;
		}
	}
	
	return pAverager;
}

void AVERAGER_NewData(AVERAGER * pAverager, int32_t newData)
{
	pAverager->data[pAverager->iWrite] = newData;
	pAverager->full |= (pAverager->iWrite == pAverager->maxIndex);
	incrementwithrollover(pAverager->iWrite, pAverager->maxIndex);
}

uint32_t AVERAGER_GetAverage(AVERAGER * pAverager)
{

	uint8_t n = 0;	
	uint32_t average;
	uint8_t count = pAverager->full ? pAverager->maxIndex : pAverager->iWrite - 1;
	
	for (n = 0; n <= count; n++)
	{
		average += pAverager->data[n];
	}
	
	return div_round(average, count+1);
}

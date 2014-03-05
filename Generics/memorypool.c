#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "memorypool.h"

#ifndef MEMORY_POOL_BYTES
#error "MEMORY_POOL_BYTES must be defined!"
#endif

static uint8_t pool[MEMORY_POOL_BYTES];
static uint8_t const * poolStart = pool;
static uint8_t const * const poolEnd = pool + MEMORY_POOL_BYTES;

uint8_t const * MEMPOOL_GetBytes(size_t bytes)
{
	uint8_t const * allocation = NULL;
	
	if (bytes > 0)
	{
		if (poolStart + bytes <= poolEnd)
		{
			allocation = poolStart;
			poolStart += bytes;
		}
	}
	
	return allocation;
}

uint16_t MEMPOOL_GetRemaining(void)
{
	return (uint16_t)(poolEnd - poolStart);
}

uint16_t MEMPOOL_GetUsed(void)
{
	return (uint16_t)(poolStart - pool);
}

#ifdef UNITY_TEST
void MEMPOOL_Reset(void)
{
	// NEVER call except when under test rig!
	poolStart = pool;
}
#endif

/*
 * Standard Library Includes
 */
 
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

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
		else
		{
			#ifdef TEST_HARNESS
			printf("MEMPOOL allocation failed requesting %zd bytes (%d remaining in pool)\n", bytes, (poolEnd - poolStart));
			#endif
			while(true) {};
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

#ifdef TEST_HARNESS
void MEMPOOL_Reset(void)
{
	// NEVER call except when under test rig!
	poolStart = pool;
	memset((void*)poolStart, 0, MEMORY_POOL_BYTES);
}
#endif

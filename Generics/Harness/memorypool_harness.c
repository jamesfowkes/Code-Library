#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include "memorypool.h"

uint8_t const * MEMPOOL_GetBytes(size_t bytes)
{
	return (uint8_t const * )malloc(bytes);
}

uint16_t MEMPOOL_GetRemaining(void)
{
	return UINT16_MAX;
}

#ifdef UNITY_TEST
void MEMPOOL_Reset(void)
{
	// NEVER call except when under test rig!
	poolStart = pool;
}
#endif

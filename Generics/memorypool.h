#ifndef _MEMORYPOOL_H_
#define _MEMORYPOOL_H_

uint8_t const * MEMPOOL_GetBytes(size_t bytes);
uint16_t MEMPOOL_GetRemaining(void);
uint16_t MEMPOOL_GetUsed(void);

#ifdef UNITY_TEST
void MEMPOOL_Reset(void);
#endif

#endif

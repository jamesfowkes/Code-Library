/*
 * Standard Library Includes
 */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <pthread.h>
#include <assert.h>
#include <time.h>
/*
 * AVR Includes (Defines and Primitives)
 */
 
#include <avr/io.h>

#include "lib_usi.h"
#include "lib_usi_harness_functions.h"

pthread_t thread;

static void *delayThread(void* ms);

void USI_Harness_TriggerISR(USI_DATA * usi, uint8_t data)
{
	usi->byte = data;
	usi->transferComplete = true;
}

void USI_Harness_KickOverflowInMs(uint32_t ms)
{
	(void)pthread_create(&thread, NULL, delayThread, &ms);
}

static void *delayThread(void* ms)
{
	struct timespec tim, tim2;
	tim.tv_sec = 0;
	tim.tv_nsec = *((uint8_t *)ms) * 1000 * 1000;
	nanosleep(&tim , &tim2);	
	USISR |= (1 << USIOIF);
	return NULL;
}


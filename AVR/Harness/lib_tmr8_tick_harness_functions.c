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
 * AVR Library Includes
 */

#include "lib_tmr8_tick.h"
#include "lib_tmr8_tick_harness_functions.h"

/*
 * Private Variables
 */
 
uint8_t TCCR0A;
uint8_t TCCR0B;
uint8_t TCNT0;
uint8_t OCR0A;
uint8_t OCR0B;
uint8_t TIMSK0;

pthread_t thread;

static void *delayThread(void*);

void TMR8_Tick_Harness_Kick(
	uint8_t ms, 
	TMR8_TICK_CONFIG * TimerConfigs[], TMR8_DELAY_CONFIG * DelayConfigs[], 
	uint8_t s_TimerCount, uint8_t s_DelayCount,
	uint16_t *pMSCounter, uint16_t *pSecondsSinceInit)
{	
	(void)pthread_create(&thread, NULL, delayThread, &ms);
	pthread_join(thread, NULL);
	
	*pMSCounter += ms;
	if (1000 >= *pMSCounter)
	{
		++(*pSecondsSinceInit);
		*pMSCounter -= 1000;
	}
	
	uint8_t i = 0;
	for (i = 0; i < s_TimerCount; ++i)
	{
		if (TimerConfigs[i]->active)
		{
			if (TimerConfigs[i]->msTick >= ms)
			{
				TimerConfigs[i]->msTick -= ms;
				if (TimerConfigs[i]->msTick < ms)
				{
					TimerConfigs[i]->triggered = true;
					TimerConfigs[i]->msTick = TimerConfigs[i]->reload;
				}
			}
		}
	}
	
	for (i = 0; i < s_DelayCount; ++i)
	{
		if (DelayConfigs[i])
		{
			if (DelayConfigs[i]->delayMs >= ms)
			{
				DelayConfigs[i]->delayMs -= ms;
				if (DelayConfigs[i]->delayMs < ms)
				{
					DelayConfigs[i]->triggered = true;
					DelayConfigs[i] = NULL;
				}
			}
		}
	}
}

static void *delayThread(void* ms)
{
	struct timespec tim, tim2;
	tim.tv_sec = 0;
	tim.tv_nsec = *((uint8_t *)ms) * 1000 * 1000;
	nanosleep(&tim , &tim2);
	
	return NULL;
}

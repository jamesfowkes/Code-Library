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

/*
 * AVR Library Includes
 */

#include "lib_tmr8_tick.h"
#include "lib_tmr8_tick_harness.h"

/*
 * Private Variables
 */

#if defined(TEST_HARNESS_TCCR0A)
uint8_t TCCR0A;
#endif
#if defined(TEST_HARNESS_TCCR0B)
uint8_t TCCR0B;
#endif
#if defined(TEST_HARNESS_TCNT0)
uint8_t TCNT0;
#endif
#if defined(TEST_HARNESS_OCR0A)
uint8_t OCR0A;
#endif
#if defined(TEST_HARNESS_OCR0B)
uint8_t OCR0B;
#endif
#if defined(TEST_HARNESS_TIMSK0)
uint8_t TIMSK0;
#endif

#if defined(TEST_HARNESS_TIMSK)
uint8_t TIMSK;
#endif

/* Timer2 Output Compare Register */
#if defined (TEST_HARNESS_OCR2)
uint8_t OCR2;
#endif

/* Timer/Counter 2 */
#if defined (TEST_HARNESS_TCNT2)
uint8_t TCNT2;
#endif

/* Timer/Counter 2 Control register */
#if defined (TEST_HARNESS_TCCR2)
uint8_t TCCR2;
#endif

/* T/C 1 Input Capture Register */
#if defined (TEST_HARNESS_ICR1)
uint16_t ICR1;
#endif
#if defined (TEST_HARNESS_ICR1L)
uint8_t ICR1L;
#endif
#if defined (TEST_HARNESS_ICR1H)
uint8_t ICR1H;
#endif

/* Timer/Counter1 Output Compare Register B */
#if defined (TEST_HARNESS_OCR1B)
uint16_t OCR1B;
#endif
#if defined (TEST_HARNESS_OCR1BL)
uint8_t OCR1BL;
#endif
#if defined (TEST_HARNESS_OCR1BH)
uint8_t OCR1BH;
#endif

/* Timer/Counter1 Output Compare Register A */
#if defined (TEST_HARNESS_OCR1A)
uint16_t OCR1A;
#endif
#if defined (TEST_HARNESS_OCR1AL)
uint8_t OCR1AL;
#endif
#if defined (TEST_HARNESS_OCR1AH)
uint8_t OCR1AH;
#endif

/* Timer/Counter 1 */
#if defined (TEST_HARNESS_TCNT1)
uint16_t TCNT1;
#endif
#if defined (TEST_HARNESS_TCNT1L)
uint8_t TCNT1L;
#endif
#if defined (TEST_HARNESS_TCNT1H)
uint8_t TCNT1H;
#endif

/* Timer/Counter 1 Control and Status Register */
#if defined (TEST_HARNESS_TCCR1B)
uint8_t TCCR1B;
#endif

/* Timer/Counter 1 Control Register */
#if defined (TEST_HARNESS_TCCR1A)
uint8_t TCCR1A;
#endif

/* Timer/Counter 0 Asynchronous Control & Status Register */
#if defined (TEST_HARNESS_ASSR)
uint8_t ASSR;
#endif

/* Output Compare Register 0 */
#if defined (TEST_HARNESS_OCR0)
uint8_t OCR0;
#endif

/* Timer/Counter 0 */
#if defined (TEST_HARNESS_TCNT0)
uint8_t TCNT0;
#endif

/* Timer/Counter 0 Control Register */
#if defined (TEST_HARNESS_TCCR0)
uint8_t TCCR0;
#endif

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

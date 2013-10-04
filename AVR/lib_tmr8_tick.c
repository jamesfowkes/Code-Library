/*
 * Standard Library Includes
 */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>

/*
 * AVR Includes (Defines and Primitives)
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

/*
 * AVR Library Includes
 */

#include "lib_clk.h"
#include "lib_tmr8.h"
#include "lib_tmr8_tick.h"
#include "lib_io.h"

/*
 * Generic Library Includes
 */
 
#include "memorypool.h"

/*
 * Private Variables
 */

typedef TMR8_TICK_CONFIG* TMR8_TICK_PTR;
typedef TMR8_DELAY_CONFIG* TMR8_DELAY_PTR;

static volatile TMR8_TICK_PTR *TimerConfigs;
static volatile TMR8_DELAY_PTR *DelayConfigs;

static volatile uint16_t secondsSinceInit = 0;
static volatile uint16_t msCounter = 0;

static uint8_t s_TimerCount = 0;
static uint8_t s_MaxTimerCount = 0;
static uint8_t s_MaxDelayCount = 0;

/*
 * Private Function Prototypes
 */

static void isrHandler(void);
 
/*
 * Public Functions
 */

/*
 * TMR8_Tick_Init
 *
 * Notes:
 * Timer source must be set before this function is called!
 *
 */
void TMR8_Tick_Init(uint8_t nTimers, uint8_t nDelays)
{

	s_MaxTimerCount = nTimers;
	s_MaxDelayCount = nDelays;
	
	TimerConfigs = (TMR8_TICK_PTR*)MEMPOOL_GetBytes(sizeof(TMR8_TICK_PTR) * nTimers);
	DelayConfigs = (TMR8_DELAY_PTR*)MEMPOOL_GetBytes(sizeof(TMR8_DELAY_PTR) * nDelays);

	if (TimerConfigs && DelayConfigs)
	{
		TMR8_SetOutputCompareMode(TMR_OUTPUTMODE_NONE, TMR_OCCHAN_A);
		TMR8_SetCountMode(TMR8_COUNTMODE_CTC);

		/* Calculate value based on clock frequency and timer division */

		TMR_SRC_ENUM tmrSource = TMR8_GetSource();

		if (tmrSource == TMR_SRC_OFF)
		{
			tmrSource = TMR_SRC_FCLK;
			TMR8_SetSource(tmrSource);
		}

		uint16_t tmr8Div = (uint16_t)TMR8_GetDivisor();
		uint16_t cpuDiv = (1 << CLK_GetPrescaler());

		uint32_t oneMilli = F_CPU / cpuDiv;
		oneMilli /= tmr8Div;
		oneMilli /= 1000;

		while((oneMilli > 255) && (tmrSource < TMR_SRC_FCLK_1024))
		{
			TMR8_SetSource(++tmrSource);
			tmr8Div = (uint16_t)TMR8_GetDivisor();
			oneMilli = F_CPU / (1 << CLK_GetPrescaler());
			oneMilli /= tmr8Div;
			oneMilli /= 1000;
		}

		TMR8_SetOutputCompareValue((uint8_t)oneMilli, TMR_OCCHAN_A);

		TMR8_InterruptControl(TMR8_INTMASK_OCMPA, true);

		secondsSinceInit = 0;
	}
}

bool TMR8_Tick_AddTimerConfig(TMR8_TICK_CONFIG * config)
{
	bool success = false;

	assert(config->reload > 0);

	if ((config->reload > 0) && (s_TimerCount < s_MaxTimerCount))
	{
		config->msTick = config->reload;

		assert(TimerConfigs[s_TimerCount] == NULL);
		TimerConfigs[s_TimerCount] = config;
		TimerConfigs[s_TimerCount]->triggered = false;
		s_TimerCount++;

		success = true;
	}

	return success;
}

bool TMR8_Tick_StartDelay(TMR8_DELAY_CONFIG * config)
{
	bool success = false;

	assert(config->delayMs > 0);

	if (config->delayMs > 0)
	{
		// Find the first NULL in the delay array
		volatile TMR8_DELAY_CONFIG * freeConfig = DelayConfigs[0];
		uint8_t i = 0;
		
		while ((freeConfig != NULL) && (i < s_MaxDelayCount))
		{
			freeConfig++;
			i++;
		}
		
		if ((freeConfig == NULL) && (i < s_MaxDelayCount))
		{
			assert(freeConfig == NULL);
			freeConfig = config;
			freeConfig->triggered = false;
			success = true;
		}
	}
	
	return success;
}

bool TMR8_Tick_TestAndClear(TMR8_TICK_CONFIG * config)
{
	cli();
	bool triggered = config->triggered;
	config->triggered = false;
	sei();
	return triggered;
}

bool TMR8_Tick_TestDelayAndClear(TMR8_DELAY_CONFIG * config)
{
	cli();
	bool triggered = config->triggered;
	config->triggered = false;
	sei();
	return triggered;
}

uint16_t TMR8_GetSecondsSinceInit(void)
{
	return secondsSinceInit;
}

static void isrHandler(void)
{
	if (1000 == ++msCounter)
	{
		++secondsSinceInit;
		msCounter = 0;
	}
	
	uint8_t i = 0;
	for (i = 0; i < s_TimerCount; ++i)
	{
		if (TimerConfigs[i]->active)
		{
			if (TimerConfigs[i]->msTick > 0)
			{
				if (--TimerConfigs[i]->msTick == 0)
				{
					TimerConfigs[i]->triggered = true;
					TimerConfigs[i]->msTick = TimerConfigs[i]->reload;
				}
			}
		}
	}
	
	for (i = 0; i < s_MaxDelayCount; ++i)
	{
		if (DelayConfigs[i])
		{
			if (DelayConfigs[i]->delayMs > 0)
			{
				if (--DelayConfigs[i]->delayMs == 0)
				{
					DelayConfigs[i]->triggered = true;
					DelayConfigs[i] = NULL;
				}
			}
		}
	}
}

#ifdef TIMER0_COMPA_vect
ISR(TIMER0_COMPA_vect)
{
	isrHandler();
}
#endif

#ifdef TIM0_COMPA_vect
ISR(TIM0_COMPA_vect)
{
	isrHandler();
}
#endif

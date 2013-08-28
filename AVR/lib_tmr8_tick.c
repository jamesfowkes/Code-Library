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
 * Private Variables
 */

#ifdef LIB_TMR8_USE_LL
static LINK_NODE * Head;
#else
static volatile TMR8_TICK_CONFIG * TimerConfig = NULL;
#endif

static volatile uint16_t secondsSinceInit = 0;
static volatile uint16_t msCounter = 0;

/*
 * Private Function Prototypes
 */

#ifdef LIB_TMR8_USE_LL
bool msListHandler(LINK_NODE * node);
#endif

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
void TMR8_Tick_Init(void)
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

bool TMR8_Tick_AddTimerConfig(TMR8_TICK_CONFIG * config)
{
	bool success = false;

	assert(config->reload > 0);

	if (config->reload > 0)
	{
		config->msTick = config->reload;

		#ifdef LIB_TMR8_USE_LL
		if (LList_ItemCount(Head) == 0)
		{
			Head = &config->Node;
			LList_Init(Head);
		}
		else
		{
			LList_Add(Head, &config->Node);
		}
		#else
		TimerConfig = config;
		#endif
		success = true;
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

uint16_t TMR8_GetSecondsSinceInit(void)
{
	return secondsSinceInit;
}

#ifdef LIB_TMR8_USE_LL
bool msListHandler(LINK_NODE * node)
{
	// Safe pointer conversion
	TMR8_TICK_CONFIG * TimerConfig = (TMR8_TICK_CONFIG*)node;

	if (TimerConfig->active)
	{
		if (TimerConfig->msTick > 0)
		{
			if (--TimerConfig->msTick == 0)
			{
				TimerConfig->msTick = TimerConfig->reload;
				TimerConfig->triggered = true;
			}
		}
	}
	return false; // continue traversing
}
#endif

#ifdef TIMER0_COMPB_vect
ISR(TIMER0_COMPB_vect)
{
	if (1000 == ++msCounter)
	{
		++secondsSinceInit;
		msCounter = 0;
	}
	#ifdef LIB_TMR8_USE_LL
	LList_Traverse(Head, msListHandler);
	#else
	if (TimerConfig->active)
	{
		if (TimerConfig->msTick > 0)
		{
			if (--TimerConfig->msTick == 0)
			{
				TimerConfig->msTick = TimerConfig->reload;
				TimerConfig->triggered = true;
			}
		}
	}
	#endif
}
#endif

#ifdef TIM0_COMPA_vect
ISR(TIM0_COMPA_vect)
{
	if (1000 == ++msCounter)
	{
		++secondsSinceInit;
		msCounter = 0;
	}

	#ifdef LIB_TMR8_USE_LL
	LList_Traverse(Head, msListHandler);
	#else
	if (TimerConfig)
	{
		if (TimerConfig->active)
		{
			if (TimerConfig->msTick > 0)
			{
				if (--TimerConfig->msTick == 0)
				{
					TimerConfig->triggered = true;
					TimerConfig->msTick = TimerConfig->reload;
				}
			}
		}
	}
	#endif
}
#endif

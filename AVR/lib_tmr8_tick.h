#ifndef _LIB_8BITTMRTICK_H_
#define _LIB_8BITTMRTICK_H_

/*
 * Requirements for this module
 */
#include "lib_tmr8.h"

#ifdef LIB_TMR8_USE_LL
#include "linkedlist.h"
#endif

struct tmr8_tick_config
{
	#ifdef LIB_TMR8_USE_LL
	LINK_NODE Node;
	#endif
	uint32_t			msTick;
	uint32_t			reload;
	bool				active;
	bool				triggered;
};
typedef struct tmr8_tick_config TMR8_TICK_CONFIG;

struct tmr8_delay_config
{
	#ifdef LIB_TMR8_USE_LL
	LINK_NODE Node;
	#endif
	uint32_t			delayMs;
	bool				triggered;
};
typedef struct tmr8_delay_config TMR8_DELAY_CONFIG;

void TMR8_Tick_Init(void);

bool TMR8_Tick_AddTimerConfig(TMR8_TICK_CONFIG * config);
bool TMR8_Tick_StartDelay(TMR8_DELAY_CONFIG * config);

uint16_t TMR8_GetSecondsSinceInit(void);

bool TMR8_Tick_TestAndClear(TMR8_TICK_CONFIG * config);
bool TMR8_Tick_TestDelayAndClear(TMR8_DELAY_CONFIG * config);
#endif

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
	TMR8_OCCHAN_ENUM	eChannel;
	uint32_t			msTick;
	uint32_t			reload;
	bool				active;
	bool				triggered;
};
typedef struct tmr8_tick_config TMR8_TICK_CONFIG;

void TMR8_Tick_Init(void);
bool TMR8_Tick_AddTimerConfig(TMR8_TICK_CONFIG * config);
uint16_t TMR8_GetSecondsSinceInit(void);

#endif

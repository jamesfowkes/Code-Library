#include <stdbool.h>

#include "linkedlist.h"

uint32_t secondsSinceInit = 0;
LINK_NODE Head;

void TMR8_Init(void)
{
	Head = NULL;
	secondsSinceInit = 0;
	
	LList_Init(Head);
	
	/* Configure timer 8 for a tick every ms */
}

bool TMR8_AddCallbackTick(TMR8_TICK_CONFIG * config)
{
	bool success = False;
	success = LList_Add( Head, config->Node );
}

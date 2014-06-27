/*
 * Standard Library Includes
 */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include "../../lib_tmr8_tick.h"

static TMR8_TICK_CONFIG tick1;
static TMR8_TICK_CONFIG tick2;

int main(int argc, char *argv[])
{
	(void)argc;
	(void)argv;
	
	setbuf(stdout, NULL);
	
	tick1.msTick = 1000;
	tick1.reload = 1000;
	tick1.active = true;
	tick1.triggered = false;

	tick2.msTick = 100;
	tick2.reload = 100;
	tick2.active = true;
	tick2.triggered = false;
	
	if (TMR8_Tick_Init(2, 0) == false)
	{
		printf("Timer not configured correctly");
		return -1;
	}
	
	TMR8_Tick_AddTimerConfig(&tick1);
	TMR8_Tick_AddTimerConfig(&tick2);

	while(true)
	{
		TMR8_Tick_Kick(50);
		if (TMR8_Tick_TestAndClear(&tick1))
		{
			printf("%u\n", TMR8_GetSecondsSinceInit());
		}
		
		if (TMR8_Tick_TestAndClear(&tick2))
		{
			printf("Tick2!");
		}
	}
	
	return 0;
}

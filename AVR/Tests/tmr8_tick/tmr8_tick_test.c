/*
 * Standard Library Includes
 */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include "../../lib_tmr8_tick.h"

static TMR8_TICK_CONFIG applicationTick;

int main(int argc, char *argv[])
{
	(void)argc;
	(void)argv;
	
	setbuf(stdout, NULL);
	
	applicationTick.msTick = 1000;
	applicationTick.reload = 1000;
	applicationTick.active = true;
	applicationTick.triggered = false;

	if (TMR8_Tick_Init(1, 0) == false)
	{
		printf("Timer not configured correctly");
		return -1;
	}
	
	TMR8_Tick_AddTimerConfig(&applicationTick);	

	while(true)
	{
		TMR8_Tick_Kick(50);
		if (TMR8_Tick_TestAndClear(&applicationTick))
		{
			printf("%u\n", TMR8_GetSecondsSinceInit());
		}
	}
	
	return 0;
}

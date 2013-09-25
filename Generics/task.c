/*
 * Standard Library Includes
 */

#include <stdbool.h>
#include <stdint.h>

/*
 * Utility Library includes
 */

#include "task.h"

/*
 * Public Functions
 */
void Task_Init(TASK * task)
{
	task->msCount = 0;
}

bool Task_Tick(TASK * task)
{
	task->msCount += task->msTick;
	
	bool trigger = (task->msCount >= task->msPerBeat);
	
	if (trigger && task->fn)
	{
		task->msCount = 0;
		task->fn();
	}
	
	return trigger;
}

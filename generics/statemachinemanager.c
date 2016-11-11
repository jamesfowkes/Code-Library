/*
 * Standard Library Includes
 */

#include <inttypes.h>
#include <stdint.h>
#include <assert.h>
#include <stdbool.h>

#include <stdio.h>

/*
 * Generic Library Includes
 */

#include "ringbuf.h"

#include "memorypool.h"

#define SM_PRIVATE_ACCESS
#include "statemachine.h"
#include "statemachinemanager.h"

/*
 * Defines
 */

#ifndef elemof
#define elemof(arr) (sizeof arr/sizeof *arr)
#endif

/*
 * Private Typedefs
 */

/*
 * Private Datatype Definitions
 */

/*
 * Private Variables
 */
 
static STATE_MACHINE_INTERNAL *StateMachines;
static SM_EVENT *EventBuffers;

static uint8_t s_StateMachineCount = 0;
static uint8_t s_MaxStateMachineCount = 0;
static uint8_t s_MaxQueue = 0;

/*
 * Public Functions
 */

bool SMM_Config(uint8_t nMachines, uint8_t nEventsInQueue)
{
	s_MaxStateMachineCount = nMachines;
	s_MaxQueue = nEventsInQueue;
	StateMachines = (STATE_MACHINE_INTERNAL *)MEMPOOL_GetBytes(sizeof(STATE_MACHINE_INTERNAL) * nMachines);
	EventBuffers = (SM_EVENT *)MEMPOOL_GetBytes(sizeof(SM_EVENT) * nMachines * nEventsInQueue);
	
	return ((StateMachines != NULL)&& (EventBuffers != NULL));
}

void SMM_GetNextMachine(STATE_MACHINE_INTERNAL **NewMachine, SM_EVENT **NewEventQueue)
{
	if (s_StateMachineCount < s_MaxStateMachineCount)
	{
		*NewMachine = &StateMachines[s_StateMachineCount];
		*NewEventQueue = &EventBuffers[s_StateMachineCount];
		
		s_StateMachineCount++;
	}
	else
	{
		*NewMachine = NULL;
		*NewEventQueue = NULL;
	}	
}

uint8_t SMM_GetMaxEventQueue(void)
{
	return s_MaxQueue;
}

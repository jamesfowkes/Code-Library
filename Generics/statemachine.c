/*
 * Standard Library Includes
 */
#include <stdint.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

/*
 * Generic Library Includes
 */

#include "ringbuf.h"

#define SM_PRIVATE_ACCESS
#include "statemachine.h"
#include "statemachinemanager.h"

/*
 * Private Typedefs
 */

/*
 * Private Function Ptototypes
 */

static bool isValidJump(uint8_t currentState, uint8_t nextEvent, SM_ENTRY const * pEntry);
static void internalInit(STATE_MACHINE_INTERNAL * Internal, SM_STATE initialState, SM_EVENT maxEvent, SM_STATE maxState, const SM_ENTRY *sm);

/*
 * Public Functions
 */

int8_t SM_Init(SM_STATE initialState, SM_EVENT maxEvent, SM_STATE maxState, const SM_ENTRY *sm)
{
	STATE_MACHINE_INTERNAL * NewMachine = NULL;
	SM_EVENT * NewEventQueue = NULL;
	
	int8_t idx = SMM_GetNextMachine(&NewMachine, &NewEventQueue);
	
	if (idx >= 0)
	{
		NewMachine->eventQueueData = NewEventQueue;
		internalInit(NewMachine, initialState, maxEvent, maxState, sm);
	}

	return idx;
}

void SM_Event(uint8_t idx, SM_EVENT event)
{
	STATE_MACHINE_INTERNAL * Internal = SMM_GetMachine(idx);
	
	if (Internal->Active && Internal->Initialised && event < Internal->MaxEvent)
	{
		Ringbuf_Put(&Internal->eventQueueBuffer, (uint8_t*)&event);
		if (Internal->Idle)
		{
			SM_Kick(idx);
		}
	}
}

void SM_SetActive(uint8_t idx, bool active)
{
	STATE_MACHINE_INTERNAL * Internal = SMM_GetMachine(idx);
	
	if (Internal->Initialised)
	{
		Internal->Active = active;
	}
}

SM_STATE SM_GetState(uint8_t idx)
{
	STATE_MACHINE_INTERNAL * Internal = SMM_GetMachine(idx);
	return Internal->CurrentState;
}

void SM_Kick(uint8_t idx)
{
	STATE_MACHINE_INTERNAL * Internal = SMM_GetMachine(idx);
	
	SM_ENTRY const *pEntry = Internal->StateTable;
	SM_EVENT nextEvent = Internal->MaxEvent;
	
	Internal->Idle = false;

	while (!Ringbuf_Empty(&Internal->eventQueueBuffer))
	{
		nextEvent = *Ringbuf_Pop_Front(&Internal->eventQueueBuffer);
		assert(nextEvent < Internal->MaxEvent);
		
		/* Find current state in table */
		while (!isValidJump(Internal->CurrentState, nextEvent, pEntry) && pEntry->OldState < Internal->MaxState) {pEntry++;}
		
		if (isValidJump(Internal->CurrentState, nextEvent, pEntry))
		{
			if (pEntry->Function)
			{
				pEntry->Function();
			}
			Internal->CurrentState = pEntry->NewState;
		}
		
		assert(Internal->CurrentState != Internal->MaxState);
	}
	
	Internal->Idle = true;
}

/*
 * Private Function Prototypes
 */

void internalInit(STATE_MACHINE_INTERNAL * Internal, SM_STATE initialState, SM_EVENT maxEvent, SM_STATE maxState, const SM_ENTRY *sm)
{
	Internal->CurrentState = initialState;
	Internal->MaxEvent = maxEvent;
	Internal->MaxState = maxState;
	Internal->StateTable = sm;
	Internal->Active = false;
	Internal->Idle = true;

	Ringbuf_Init(&Internal->eventQueueBuffer, (uint8_t*)Internal->eventQueueData, sizeof(SM_EVENT), SMM_GetMaxEventQueue(), false);

	Internal->Initialised = true;
}

bool isValidJump(uint8_t currentState, uint8_t nextEvent, SM_ENTRY const * pEntry)
{
	return (pEntry->OldState == currentState) && (pEntry->Event == nextEvent);
}

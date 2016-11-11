/*
 * Standard Library Includes
 */
#include <stdint.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>

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

static bool isValidJump(const SM_STATE *const currentState, uint8_t nextEvent, SM_ENTRY const * pEntry);

/*
 * Public Functions
 */

void SM_Init(STATE_MACHINE * sm)
{
	STATE_MACHINE_INTERNAL * NewMachine = NULL;
	SM_EVENT * NewEventQueue = NULL;
	
	SMM_GetNextMachine(&NewMachine, &NewEventQueue);
	
	if (NewMachine)
	{
		sm->Internal = NewMachine;
		NewMachine->eventQueueData = NewEventQueue;
		Ringbuf_Init(&NewMachine->eventQueueBuffer, (uint8_t*)NewMachine->eventQueueData, sizeof(SM_EVENT), SMM_GetMaxEventQueue(), false);
		NewMachine->Initialised = true;
	}
}

void SM_Event(STATE_MACHINE * sm, SM_EVENT event)
{
	if (sm->Active && sm->Internal->Initialised && event < sm->NumEvents)
	{
		Ringbuf_Put(&sm->Internal->eventQueueBuffer, (uint8_t*)&event);
		if (sm->Idle)
		{
			SM_Kick(sm);
		}
	}
}

void SM_SetActive(STATE_MACHINE * sm, bool active)
{
	if (sm->Internal->Initialised)
	{
		sm->Active = active;
	}
}

/*SM_STATEID SM_GetState(STATE_MACHINE * sm)
{
	return sm->CurrentState->ID;
}
*/

void SM_Kick(STATE_MACHINE * sm)
{
	SM_ENTRY const *pEntry = sm->StateTable;

	SM_EVENT nextEvent = sm->NumEvents;
	SM_STATEID oldStateId = sm->CurrentState->ID; // Used to pass to onEnter , onLeave and state change functions
	sm->Idle = false;

	while (!Ringbuf_Empty(&sm->Internal->eventQueueBuffer))
	{
		nextEvent = *Ringbuf_Pop_Front(&sm->Internal->eventQueueBuffer);
		assert(nextEvent < sm->NumEvents);
		
		oldStateId = sm->CurrentState->ID;

		/* Find current state in table */
		while (!isValidJump(sm->CurrentState, nextEvent, pEntry) && pEntry->OldState->ID < sm->NumStates) {pEntry++;}
		
		if (isValidJump(sm->CurrentState, nextEvent, pEntry))
		{
			if (sm->CurrentState->onLeave)
			{
				sm->CurrentState->onLeave(oldStateId, pEntry->NewState->ID, nextEvent);
			}
			
			if (pEntry->Function)
			{
				pEntry->Function(oldStateId, pEntry->NewState->ID, nextEvent);
			}
			
			sm->CurrentState = pEntry->NewState;
			
			if (sm->CurrentState->onEnter)
			{
				sm->CurrentState->onEnter(oldStateId, pEntry->NewState->ID, nextEvent);
			}
		}
		
		assert(sm->CurrentState->ID <= sm->NumStates);
	}
	
	sm->Idle = true;
}

/*
 * Private Function Prototypes
 */

bool isValidJump(const SM_STATE *const currentState, uint8_t nextEvent, SM_ENTRY const * pEntry)
{
	return (pEntry->OldState == currentState) && (pEntry->Event == nextEvent);
}

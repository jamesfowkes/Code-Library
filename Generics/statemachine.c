/*
 * Standard Library Includes
 */
#include <inttypes.h>
#include <stdint.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

/*
 * Generic Library Includes
 */

#include "ringbuf.h"
#include "statemachine.h"
#include "statemachine_common.h"

/*
 * External Functions
 */

extern STATE_MACHINE_INTERNAL * SM_GetMachineInternal(uint8_t idx);

/*
 * Public Functions
 */

void SM_Event(uint8_t idx, SM_EVENT event)
{
	
	STATE_MACHINE_INTERNAL * Internal = SM_GetMachineInternal(idx);
	
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
	STATE_MACHINE_INTERNAL * Internal = SM_GetMachineInternal(idx);
	
	if (Internal->Initialised)
	{
		Internal->Active = active;
	}
}

SM_STATE SM_GetState(uint8_t idx)
{
	STATE_MACHINE_INTERNAL * Internal = SM_GetMachineInternal(idx);
	return Internal->CurrentState;
}

void SM_Kick(uint8_t idx)
{
	STATE_MACHINE_INTERNAL * Internal = SM_GetMachineInternal(idx);
	
	SM_ENTRY const *pEntry = Internal->StateTable;
	SM_EVENT nextEvent = Internal->MaxEvent;
	
	Internal->Idle = false;

	while (!Ringbuf_Empty(&Internal->eventQueueBuffer))
	{
		nextEvent = *Ringbuf_Pop_Front(&Internal->eventQueueBuffer);
		assert(nextEvent < Internal->MaxEvent);
		
		/* Find current state in table */
		while (!SM_IsValidJump(Internal->CurrentState, nextEvent, pEntry) && pEntry->OldState < Internal->MaxState) {pEntry++;}
		
		if (SM_IsValidJump(Internal->CurrentState, nextEvent, pEntry))
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

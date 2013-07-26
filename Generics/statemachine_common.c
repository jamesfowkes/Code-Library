/*
 * Standard Library Includes
 */
#include <inttypes.h>
#include <stdint.h>
#include <assert.h>
#include <stdio.h>
#include <stdbool.h>

/*
 * Generic Library Includes
 */

#include "ringbuf.h"
#include "statemachine.h"
#include "statemachine_common.h"

/*
 * Defines
 */
#ifndef elemof
#define elemof(arr) (sizeof arr/sizeof *arr)
#endif

/*
 * Public Functions
 */

void SM_InternalInit(STATE_MACHINE_INTERNAL * Internal, SM_STATE initialState, SM_EVENT maxEvent, SM_STATE maxState, const SM_ENTRY *sm)
{
	Internal->CurrentState = initialState;
	Internal->MaxEvent = maxEvent;
	Internal->MaxState = maxState;
	Internal->StateTable = sm;
	Internal->Active = false;
	Internal->Idle = true;

	Ringbuf_Init(&Internal->eventQueueBuffer, (uint8_t*)Internal->eventQueueData, sizeof(SM_EVENT), MAX_EVENT_QUEUE);

	Internal->Initialised = true;
}

bool SM_IsValidJump(uint8_t currentState, uint8_t nextEvent, SM_ENTRY const * pEntry)
{
	return (pEntry->OldState == currentState) && (pEntry->Event == nextEvent);
}

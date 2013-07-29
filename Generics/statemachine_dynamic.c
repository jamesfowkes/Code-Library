/*
 * Standard Library Includes
 */

#include <inttypes.h>
#include <stdint.h>
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

/*
 * Generic Library Includes
 */

#include "linkedlist.h"
#include "ringbuf.h"
#include "statemachine.h"
#include "statemachine_common.h"

/*
 * Defines
 */

#ifndef MAX_EVENT_QUEUE
	#error "MAX_EVENT_QUEUE must be defined for a state machine implementation."
#endif

/*
 * Private Datatype Definitions
 */

struct state_machine_control
{
	LINK_NODE				Node;
	uint8_t					Index;
	STATE_MACHINE_INTERNAL	Internal;
};
typedef struct state_machine_control SM_CONTROL;

/*
 * Private Function Prototypes
 */

bool MatchIndex(LINK_NODE * node);

/*
 * Private Variables
 */

static LINK_NODE  * StateMachineListHead = NULL;

static uint8_t IndexToMatch = 0;

/*
 * Public Functions
 */

uint8_t SM_Init(SM_STATE initialState, SM_EVENT maxEvent, SM_STATE maxState, const SM_ENTRY *sm)
{
	
	SM_CONTROL * NewMachine = malloc(sizeof(SM_CONTROL));
	uint8_t NewIndex = 0;

	if (LList_ItemCount(StateMachineListHead) == 0)
	{
		StateMachineListHead = &NewMachine->Node;
		LList_Init(StateMachineListHead);
	}
	else
	{
		NewIndex = ((SM_CONTROL*)LList_Tail(StateMachineListHead))->Index + 1;
	}
	
	if (NewMachine)
	{
		NewMachine->Internal.eventQueueData = malloc(MAX_EVENT_QUEUE);
		SM_InternalInit(&NewMachine->Internal, initialState, maxEvent, maxState, sm);
		LList_Add(StateMachineListHead, &NewMachine->Node);
	}
	
	return NewIndex;
}

STATE_MACHINE_INTERNAL * SM_GetMachineInternal(uint8_t idx)
{
	IndexToMatch = idx;
	SM_CONTROL * ListItem = (SM_CONTROL * )LList_Traverse(StateMachineListHead, MatchIndex);
	return &ListItem->Internal;
}

bool MatchIndex(LINK_NODE * node)
{
	SM_CONTROL * ListItem = (SM_CONTROL*)node;

	return (IndexToMatch == ListItem->Index);
}

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
#include "statemachine.h"
#include "statemachine_common.h"

/*
 * Defines
 */

#ifndef elemof
#define elemof(arr) (sizeof arr/sizeof *arr)
#endif

#ifndef NUM_STATE_MACHINES
	#error "NUM_STATE_MACHINES must be defined for a static state machine implementation."
#else

	#ifndef MAX_EVENT_QUEUE
		#error "MAX_EVENT_QUEUE must be defined for a state machine implementation."
	#else
		static SM_CONTROL StateMachines[NUM_STATE_MACHINES];
		static SM_EVENT EventBuffers[NUM_STATE_MACHINES][MAX_EVENT_QUEUE];
	#endif
#endif

/*
 * Private Datatype Definitions
 */

struct state_machine_control {
	STATE_MACHINE_INTERNAL Internal;
};
typedef struct state_machine_control SM_CONTROL;

/*
 * Private Variables
 */

static uint8_t StateMachineCount;

/*
 * Public Functions
 */

uint8_t SM_Init(SM_STATE initialState, SM_EVENT maxEvent, SM_STATE maxState, const SM_ENTRY *sm)
{
	SM_CONTROL * NewMachine = NULL;
	uint8_t idx = UINT8_MAX;

	if (StateMachineCount < NUM_STATE_MACHINES) {
		idx = StateMachineCount++;

		NewMachine = &StateMachines[idx];

		StateMachines[idx].Internal.eventQueueData = EventBuffers[idx];

		SM_InternalInit(&NewMachine->Internal, initialState, maxEvent, maxState, sm);

	}

	return idx;
}

STATE_MACHINE_INTERNAL * SM_GetMachineInternal(uint8_t idx)
{
	return &(StateMachines[idx].Internal);
}

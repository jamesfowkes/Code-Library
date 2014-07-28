#ifndef _STATEMACHINE_H_
#define _STATEMACHINE_H_

typedef int8_t SM_STATEID;
typedef uint8_t SM_EVENT;

typedef void(*SM_FUNCTION)(SM_STATEID, SM_STATEID, SM_EVENT);

struct state_machine_state
{
	SM_STATEID ID;
	SM_FUNCTION onLeave;
	SM_FUNCTION onEnter;
};
typedef struct state_machine_state SM_STATE;

struct state_machine_entry
{
	const SM_STATE *const	OldState;
	SM_EVENT				Event;
	SM_FUNCTION				Function;
	const SM_STATE *const 	NewState;
};
typedef struct state_machine_entry SM_ENTRY;

// Forward declaration of internal struct
typedef struct state_machine_internal STATE_MACHINE_INTERNAL;

struct state_machine
{
	const SM_STATE 			*CurrentState;
	const SM_ENTRY 			*StateTable;
	SM_EVENT				NumEvents;
	SM_STATEID				NumStates;
	bool					Active;
	bool					Idle;
	STATE_MACHINE_INTERNAL	*Internal;
};
typedef struct state_machine STATE_MACHINE;

/* These macros should be used to define the states and events for a state machine */
#define DEFINE_STATE_MACHINE(name, initialState) static STATE_MACHINE name = {&(name ## States[initialState]), name ## TransitionTable, MAX_STATES, MAX_EVENTS, false, true, NULL}
#define STATE_MACHINE(x) MAX_EVENTS, MAX_STATES, x
#define DEFINE_SM_STATES(...) enum states {__VA_ARGS__, MAX_STATES}; typedef enum states STATES
#define DEFINE_SM_EVENTS(...) enum events {__VA_ARGS__, MAX_EVENTS}; typedef enum events EVENTS
#define DEFINE_STATE_TRANSITIONS(x) static const SM_ENTRY x ## TransitionTable[]
#define DEFINE_STATES(x) static SM_STATE x ## States[]
#define STATE_TRANSITION(x, old, event, fn, new) {&x##States[old], event, fn, &x##States[new]}
/* State machine template:
	Copy-and-paste this into a .c file to define a state machine.
	Replace items in < > brackets with application specific identifiers
	
DEFINE_SM_STATES( <Comma-separated list of state IDs>);
DEFINE_SM_EVENTS( <Comma-separated list of event IDs>);

DEFINE_STATES(<stateTableName>) = {
	{<stateID>, <onLeaveCallback>, <onEnterCallback>},
	{<stateID>, <onLeaveCallback>, <onEnterCallback>},
	...
};

DEFINE_STATE_TRANSITIONS(<stateMachineName>) = {
	STATE_TRANSITION(sm, <currentStateID>, <eventID>, <handlerFunction>, <newStateID>),
	STATE_TRANSITION(sm, <currentStateID>, <eventID>, <handlerFunction>, <newStateID>),
	...
};

DEFINE_STATE_MACHINE(<stateMachineName, <firstStateID>]);

*/

#ifdef SM_PRIVATE_ACCESS

struct state_machine_internal
{
	bool			Initialised;
	RING_BUFFER		eventQueueBuffer;
	SM_EVENT		*eventQueueData;
};

#endif

/*
 * Public Function Prototypes
 */

void SM_Init(STATE_MACHINE * sm);
void SM_Event(STATE_MACHINE * sm, SM_EVENT event);
void SM_Kick(STATE_MACHINE * sm);
void SM_SetActive(STATE_MACHINE * sm, bool active);
SM_STATEID SM_GetState(STATE_MACHINE * sm);

#endif

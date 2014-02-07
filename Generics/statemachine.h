#ifndef _STATEMACHINE_H_
#define _STATEMACHINE_H_

typedef uint8_t SM_STATEID;
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

#ifdef SM_PRIVATE_ACCESS

struct state_machine_internal
{
	const SM_STATE *CurrentState;
	SM_EVENT		MaxEvent;
	SM_STATEID		MaxStateID;
	bool			Active;
	bool			Idle;
	const SM_ENTRY *StateTable;
	bool			Initialised;
	RING_BUFFER		eventQueueBuffer;
	SM_EVENT		*eventQueueData;
};
typedef struct state_machine_internal STATE_MACHINE_INTERNAL;

#endif

/*
 * Public Function Prototypes
 */

int8_t SM_Init(const SM_STATE *initialState, SM_EVENT maxEvent, SM_STATEID maxStateID, const SM_ENTRY *sm);
void SM_Event(uint8_t idx, SM_EVENT event);
void SM_Kick(uint8_t idx);
void SM_SetActive(uint8_t idx, bool active);
SM_STATEID SM_GetState(uint8_t idx);

#endif

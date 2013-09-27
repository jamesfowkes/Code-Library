#ifndef _STATEMACHINE_H_
#define _STATEMACHINE_H_

typedef void(*SM_FUNCTION)(void);

typedef uint8_t SM_STATE;
typedef uint8_t SM_EVENT;

struct state_machine_entry
{
	SM_STATE	OldState;
	SM_EVENT	Event;
	SM_FUNCTION	Function;
	SM_STATE	NewState;
};
typedef struct state_machine_entry SM_ENTRY;

#ifdef SM_PRIVATE_ACCESS

struct state_machine_internal
{
	SM_STATE		CurrentState;
	SM_EVENT		MaxEvent;
	SM_STATE		MaxState;
	bool			Active;
	bool			Idle;
	const SM_ENTRY	*StateTable;
	bool			Initialised;
	RING_BUFFER		eventQueueBuffer;
	SM_EVENT		*eventQueueData;
};
typedef struct state_machine_internal STATE_MACHINE_INTERNAL;

#endif

/*
 * Public Function Prototypes
 */

int8_t SM_Init(SM_STATE initialState, SM_EVENT maxEvent, SM_STATE maxState, const SM_ENTRY *sm);
void SM_Event(uint8_t idx, SM_EVENT event);
void SM_Kick(uint8_t idx);
void SM_SetActive(uint8_t idx, bool active);
SM_STATE SM_GetState(uint8_t idx);

#endif

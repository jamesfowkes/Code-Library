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

void SM_InternalInit(STATE_MACHINE_INTERNAL * Internal, SM_STATE initialState, SM_EVENT maxEvent, SM_STATE maxState, const SM_ENTRY *sm);
bool SM_IsValidJump(uint8_t currentState, uint8_t nextEvent, SM_ENTRY const * pEntry);

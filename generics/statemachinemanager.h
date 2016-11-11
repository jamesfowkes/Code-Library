#ifndef _STATEMACHINEMANAGER_H_
#define _STATEMACHINEMANAGER_H_

bool SMM_Config(uint8_t nMachines, uint8_t nEventsInQueue);

#ifdef SM_PRIVATE_ACCESS
void SMM_GetNextMachine(STATE_MACHINE_INTERNAL **NewMachine, SM_EVENT **NewEventQueue);
uint8_t SMM_GetMaxEventQueue(void);
#endif

#endif

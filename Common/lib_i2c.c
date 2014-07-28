/*
 * Standard Library Includes
 */
#include <stdbool.h>
#include <stddef.h>

#include <stdlib.h>
#include <assert.h>

#include <stdint.h>

/*
 * Common and Generic Includes
 */

#include "lib_i2c_common.h"
#include "lib_i2c_private.h"
#include "lib_i2c_defs.h"

#include "util_macros.h"

static I2C_STATEMACHINE * state_machines[4] =
{ NULL, NULL, NULL, NULL };
static volatile I2C_STATEMACHINE * active_sm = NULL;
static I2C_TRANSFER_DATA * activeTransfer = NULL;

static I2C_SLAVE_HANDLER getSlaveTransferData = NULL;

static uint8_t s_ThisSlaveAddress = 0;

bool I2C_Init(I2C_SLAVE_HANDLER slaveHandler)
{

	// Get pointers to the state machines for each mode
#ifdef I2C_MT
	state_machines[I2CM_MT] = I2C_MT_GetSM();
#endif
#ifdef I2C_MR
	state_machines[I2CM_MR] = I2C_MR_GetSM();
#endif
#ifdef I2C_ST
	state_machines[I2CM_ST] = I2C_ST_GetSM();
#endif
#ifdef I2C_SR
	state_machines[I2CM_SR] = I2C_SR_GetSM();
#endif

	getSlaveTransferData = slaveHandler;

	return true;
}

bool I2C_IsActive(void)
{
	return (active_sm != NULL);
}

void I2C_SetSlaveAddress(uint8_t slaveAddress)
{
	s_ThisSlaveAddress = slaveAddress;
	setAddress(slaveAddress);
}

bool I2C_StartMaster(I2C_TRANSFER_DATA * newTransferData, bool read, bool repeatStart)
{
	bool success = false;
	if (newTransferData)
	{
		if ((active_sm == NULL) && newTransferData->buffer && newTransferData->totalBytes)
		{
			active_sm = state_machines[read ? I2CM_MR : I2CM_MT];

			if (!active_sm->busy)
			{
				activeTransfer = newTransferData;
				read ? I2C_MR_SetTransferData(activeTransfer) : I2C_MT_SetTransferData(activeTransfer);
				read ? I2C_MR_SetRepeatStart(repeatStart) : I2C_MT_SetRepeatStart(repeatStart);
				read ? I2C_MR_Start() : I2C_MT_Start();
				success = true;
			}
		}
	}
	return success;
}

bool I2C_StartMasterFromRS(I2C_TRANSFER_DATA * newTransferData, bool read, bool repeatStart)
{
	// When a previous read or write has left the bus in the start condition (repeated start)
	// this function can be called to start another transfer

	bool success = false;

	if (newTransferData)
	{
		if ((active_sm == NULL) && newTransferData->buffer && newTransferData->totalBytes)
		{
			active_sm = state_machines[read ? I2CM_MR : I2CM_MT];

			if (!active_sm->busy)
			{
				activeTransfer = newTransferData;
				read ? I2C_MR_SetTransferData(activeTransfer) : I2C_MT_SetTransferData(activeTransfer);
				read ? I2C_MR_SetRepeatStart(repeatStart) : I2C_MT_SetRepeatStart(repeatStart);
				// Send in the repeated start event
				I2C_New_Event(I2C_REP_START);
				success = true;
			}
		}
	}
	return success;
}

bool I2C_RxBufferFull(void)
{
	return (activeTransfer->bytesTransferred == activeTransfer->totalBytes);
}

bool I2C_TxBufferUsed(void)
{
	return (activeTransfer->bytesTransferred == activeTransfer->totalBytes);
}

void I2C_Done(bool success)
{
	// Clear the busy flag and reset state before clearing the pointer
	active_sm->busy = false;
	active_sm->currentState = I2CS_IDLE;

	active_sm = NULL;
	activeTransfer->success = success;
	activeTransfer->callback(activeTransfer);
}

void I2C_New_Event(I2C_EVENT event)
{

	if (active_sm == NULL)
	{
		/* Since not currently executing a state machine, this event
		should be a slave address ack or nack */
		switch (event)
		{
		case I2C_ST_SLA_ACK:
			active_sm = state_machines[I2CM_ST];
			activeTransfer = getSlaveTransferData(false);
			break;
		case I2C_SR_SLA_ACK:
			active_sm = state_machines[I2CM_SR];
			activeTransfer = getSlaveTransferData(true);
			break;
		default:
			break;
		}

		// Change the event to generic SLA_ACK for state machine handling
		event = I2C_SLA_ACK;

	}

	if (active_sm)
	{
		active_sm->busy = true;

		I2C_STATEMACHINEENTRY const * search = active_sm->entries;

		// Find state

		while (search->currentState != active_sm->currentState)
		{
			search++;
		}

		// Find event
		while (search->event != event)
		{
			search++;
		}

		if (search->event == event)
		{
			active_sm->currentEvent = event;
			search->handler();
			active_sm->currentState = search->nextState;
		}

		if (active_sm) // Might have been cleared by I2C_Done()
		{
			active_sm->busy = false;
		}
	}
}



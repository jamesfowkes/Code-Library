/*
 * Standard Library Includes
 */
#include <stdbool.h>
#include <stddef.h>

#include <stdlib.h>
#include <assert.h>

#include <stdint.h>

#include "lib_i2c_test.h"

/*
 * Common and Generic Includes
 */
#include "lib_i2c_common.h"
#include "lib_i2c_private.h"

static void new_slave_event(uint8_t event);
static void i2c_sm_event(uint8_t event);

I2C_STATEMACHINE * state_machines[4];
I2C_STATEMACHINE * active_sm = NULL;
I2C_TRANSFER_DATA * activeTransfer = NULL;

static I2C_SLAVE_HANDLER getSlaveTransferData = NULL;

bool I2C_Init(I2C_SLAVE_HANDLER slaveHandler)
{
	// Get pointers to the state machines for each mode
	state_machines[I2CM_MT] = I2C_MT_GetSM();
	state_machines[I2CM_MR] = I2C_MR_GetSM();
	state_machines[I2CM_ST] = I2C_ST_GetSM();
	state_machines[I2CM_SR] = I2C_SR_GetSM();
	
	getSlaveTransferData = slaveHandler;
	
	return true;
}

bool I2C_StartMaster(I2C_TRANSFER_DATA * newTransferData, bool read)
{
	bool success = false;
	
	if ((active_sm = NULL) && newTransferData->buffer && newTransferData->totalBytes)
	{
		active_sm = state_machines[read ? I2CM_MR : I2CM_MT];
		
		if (!active_sm->busy)
		{
			if (newTransferData)
			{
				activeTransfer = newTransferData;
				read ? I2C_MR_Start() : I2C_MT_Start();
				success = true;
			}
		}
	}

	return success;
}

bool I2C_BufferFull(void)
{
	return (activeTransfer->bytesTransferred == (activeTransfer->totalBytes - 1));
}

bool I2C_BufferUsed(void)
{
	return (activeTransfer->bytesTransferred == (activeTransfer->totalBytes - 1));
}

I2C_TRANSFER_DATA * data(void)
{
	return activeTransfer;
}

void I2C_Done(bool success)
{
	// Clear the busy flag before clearing the pointer
	active_sm->busy = false;
	active_sm = NULL;
	activeTransfer->success = success;
	activeTransfer->callback(activeTransfer);
}

static void i2c_sm_event(uint8_t event)
{
	if (active_sm)
	{
		active_sm->busy = true;
		
		I2C_STATEMACHINEENTRY *search = active_sm->entries;
		
		// Find state
		while (search->currentState != active_sm->currentState)
		{
			search++;
		}
		
		// Find event
		while (search->currentState == active_sm->currentState)
		{
			if (search->event == event) {break;}
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

void new_slave_event(uint8_t event)
{
	switch (event)
	{
	case TW_ST_SLA_ACK:
		active_sm = state_machines[I2CM_ST];
		activeTransfer = getSlaveTransferData(false);
		i2c_sm_event(TW_ST_SLA_ACK);
		break;
	case TW_SR_SLA_ACK:
		active_sm = state_machines[I2CM_SR];
		activeTransfer = getSlaveTransferData(true);
		i2c_sm_event(TW_SR_SLA_ACK);
		break;
	}
}

ISR(TWI_vect)
{
	uint8_t status = TWSR;
	if (status & TWINT)
	{
		status = status & 0xF8;
		
		if (status != TW_NO_INFO)
		{
			if (active_sm)
			{
				while (active_sm->busy) {continue;}
				i2c_sm_event(status);
			}
			else
			{	
				new_slave_event(status);
			}
		}
	}
}

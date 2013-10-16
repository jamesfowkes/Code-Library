/*
 * Standard Library Includes
 */
#include <stdbool.h>
#include <stddef.h>

#include <stdlib.h>
#include <assert.h>

#include <stdint.h>

/*
 * AVR Includes (Defines and Primitives)
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/twi.h>

/*
 * Common and Generic Includes
 */

#include "lib_i2c_common.h"
#include "lib_i2c_private.h"
#include "lib_i2c_defs.h"

#include "lib_io.h"
#include "util_macros.h"

static void new_slave_event(uint8_t event);
static void i2c_sm_event(uint8_t event);

static I2C_STATEMACHINE * state_machines[4] =
{ NULL, NULL, NULL, NULL };
static volatile I2C_STATEMACHINE * active_sm = NULL;
static I2C_TRANSFER_DATA * activeTransfer = NULL;

static I2C_SLAVE_HANDLER getSlaveTransferData = NULL;

static uint8_t s_ThisSlaveAddress = 0;

static bool s_bTWINTSet;
static uint8_t s_lastTWSR;

bool I2C_Init(I2C_SLAVE_HANDLER slaveHandler)
{
	s_bTWINTSet = false;

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

void I2C_SetSlaveAddress(uint8_t slaveAddress)
{
	s_ThisSlaveAddress = slaveAddress;
	TWAR = (slaveAddress << 1) & 0xFE;
}

void I2C_AcceptGCALL(bool accept)
{
	if (accept)
	{
		TWAR |= 0x01;
	}
	else
	{
		TWAR &= 0xF8;
	}
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
				read ? I2C_MR_Start(activeTransfer, repeatStart) : I2C_MT_Start(activeTransfer, repeatStart);
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

			// Jump the active state machine to its transferring state
			active_sm->currentState = I2CS_TRANSFERRING;

			if (!active_sm->busy)
			{
				activeTransfer = newTransferData;
				// Pretend the hardware got a data ACK to start transferring
				read ? i2c_sm_event(TW_MR_DATA_ACK) : i2c_sm_event(TW_MT_DATA_ACK);
				success = true;
			}
		}
	}
	return success;
}

void I2C_SetPrescaler(uint8_t divisor)
{
	TWBR = divisor;
}

bool I2C_BufferFull(void)
{
	return (activeTransfer->bytesTransferred == activeTransfer->totalBytes);
}

bool I2C_BufferUsed(void)
{
	return (activeTransfer->bytesTransferred == activeTransfer->totalBytes);
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

void I2C_Task(void)
{
	if (s_bTWINTSet)
	{
		s_bTWINTSet = false;

		uint8_t status = s_lastTWSR;
		status = status & 0xF8;

		if (status != TW_NO_INFO)
		{
			if (active_sm)
			{
				i2c_sm_event(status);
			}
			else
			{
				new_slave_event(status);
			}
		}
	}
}

ISR(TWI_vect)
{
	while (!(TWCR & (1<<TWINT))) { continue; }
	s_bTWINTSet = true;
	s_lastTWSR = TWSR;
}

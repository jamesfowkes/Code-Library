/*
 * Standard Library Includes
 */
#include <stdbool.h>
#include <stddef.h>

#include <stdlib.h>
#include <assert.h>

#include <stdint.h>

#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/extensions/HelperMacros.h>

/*
 * Common and Generic Includes
 */

#include "lib_i2c_common.h"
#include "lib_i2c_defs.h"

static void sendAddress(void);
static void sendFirstAck(void);
static void getNextByte(void);
static void done(void);
static void finish(void);
static void errorCondition(void);

static const I2C_STATEMACHINEENTRY sm_entries[] =
{
	{I2CS_IDLE,		I2C_START,		sendAddress,	I2CS_ADDRESSING		},
	{I2CS_IDLE,		I2C_REP_START,		sendAddress,	I2CS_ADDRESSING		},
	{I2CS_IDLE,		I2C_BUS_ERROR,		errorCondition,	I2CS_IDLE		},
	
	{I2CS_ADDRESSING,	I2C_MR_SLA_ACK,	sendFirstAck,	I2CS_TRANSFERRING	},
	{I2CS_ADDRESSING,	I2C_MR_SLA_NACK,	errorCondition,	I2CS_IDLE		},
	
	{I2CS_ADDRESSING,	I2C_BUS_ERROR,		errorCondition,	I2CS_IDLE		},
	
	{I2CS_TRANSFERRING,	I2C_MR_DATA_ACK,	getNextByte,	I2CS_TRANSFERRING	},
	{I2CS_TRANSFERRING,	I2C_MR_DATA_NACK,	done,		I2CS_IDLE		},
	{I2CS_TRANSFERRING, 	I2C_REP_START, 	finish,		I2CS_IDLE		},
	{I2CS_TRANSFERRING,	I2C_MR_ARB_LOST,	errorCondition,	I2CS_IDLE		},
	
	{I2CS_TRANSFERRING,	I2C_BUS_ERROR,		errorCondition,	I2CS_IDLE		},
};

static I2C_STATEMACHINE sm = {false, 0, I2CS_IDLE, sm_entries};
static I2C_TRANSFER_DATA * pTransfer = NULL;

static bool s_repeatStart = false;

I2C_STATEMACHINE * I2C_MR_GetSM(void) {return &sm;}

void I2C_MR_SetTransferData(I2C_TRANSFER_DATA * transfer)
{
	pTransfer = transfer;
	pTransfer->bytesTransferred = 0;
}

void I2C_MR_Start(void)
{
	start();
}

void I2C_MR_SetRepeatStart(bool repeatStart)
{
	s_repeatStart = repeatStart;
}

static void sendAddress(void)
{
	uint8_t address = (pTransfer->address << 1);
	address |= 0x01; // For reading
	I2C_PLATFORM_SET_ADDR(address);
	send();
}

static void sendFirstAck(void)
{
	if (!I2C_RxBufferFull()) { ack() } else { nack(); }
}

static void getNextByte(void)
{
	I2C_PLATFORM_GET_DATA(pTransfer->buffer[pTransfer->bytesTransferred]);

	(pTransfer->bytesTransferred)++;
	// More bytes to receive
	if (!I2C_RxBufferFull()) { ack(); } else { nack(); }
}

static void done(void)
{
	// No more bytes to receive after this
	if (s_repeatStart)
	{
		s_repeatStart = false;
		start();
	}
	else
	{
		stop();
		I2C_Done(true);
	}
}

static void finish(void)
{
	I2C_Done(true);
}

static void errorCondition(void)
{
	stop();
	I2C_Done(false);
}

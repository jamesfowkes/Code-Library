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

static void startSR(void);
static void getNextByte(void);
static void sendNack(void);
static void finish(void);
static void errorCondition(void);

static I2C_STATEMACHINEENTRY sm_entries[] = 
{
	{I2CS_IDLE,			I2C_SLA_ACK,			startSR,		I2CS_TRANSFERRING	},
	{I2CS_IDLE,			I2C_GCALL_ACK,			startSR,		I2CS_TRANSFERRING	},
	{I2CS_IDLE,			I2C_ARB_LOST_SLA_ACK,	startSR,		I2CS_TRANSFERRING	},
	{I2CS_IDLE,			I2C_ARB_LOST_GCALL_ACK,	startSR,		I2CS_TRANSFERRING	},
	
	{I2CS_IDLE,			I2C_BUS_ERROR,				errorCondition,	I2CS_IDLE			},
	
	{I2CS_TRANSFERRING,	I2C_DATA_ACK,			getNextByte,	I2CS_TRANSFERRING	},
	{I2CS_TRANSFERRING,	I2C_GCALL_DATA_ACK,		getNextByte,	I2CS_TRANSFERRING	},
	{I2CS_TRANSFERRING,	I2C_DATA_NACK,			sendNack,		I2CS_TRANSFERRING	},
	{I2CS_TRANSFERRING,	I2C_GCALL_DATA_NACK,		sendNack,		I2CS_TRANSFERRING	},
	{I2CS_TRANSFERRING,	I2C_STOP,				finish,			I2CS_IDLE			},
	
	{I2CS_TRANSFERRING,	I2C_BUS_ERROR,				errorCondition,	I2CS_IDLE			},
};

static I2C_STATEMACHINE sm = {false, 0, I2CS_IDLE, sm_entries};

I2C_STATEMACHINE * I2C_SR_GetSM(void) {return &sm;}

static void startSR(void)
{
	ack();
}

static void getNextByte(void)
{
	// Store the data, then ack or just nack
	if (!I2C_BufferFull())
	{
		readData(data()->buffer[data()->bytesTransferred++]);
		ack();
	}
	else
	{
		nack();
	}
}

static void sendNack(void)
{
	nack();
}

static void finish(void)
{
	stop();
	release();
	I2C_Done(true);
}

static void errorCondition(void)
{
	stop();
	I2C_Done(false);
}

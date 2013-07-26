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
#include "lib_i2c_defs.h"

static void sendAddress(void);
static void errorCondition(void);
static void txNextByte(void);

static I2C_STATEMACHINEENTRY sm_entries[] = 
{
	{I2CS_IDLE,			TW_START,			sendAddress,	I2CS_ADDRESSING		},
	{I2CS_IDLE,			TW_REP_START,		sendAddress,	I2CS_ADDRESSING		},
	{I2CS_IDLE,			TW_BUS_ERROR,		errorCondition,	I2CS_IDLE			},
	
	{I2CS_ADDRESSING,	TW_MT_SLA_ACK,		txNextByte,		I2CS_TRANSFERRING	},
	{I2CS_ADDRESSING,	TW_MT_SLA_NACK,		errorCondition,	I2CS_IDLE			},
	
	{I2CS_ADDRESSING,	TW_BUS_ERROR,		errorCondition,	I2CS_IDLE			},
	
	{I2CS_TRANSFERRING,	TW_MT_DATA_ACK,		txNextByte,		I2CS_TRANSFERRING	},
	{I2CS_TRANSFERRING,	TW_MT_DATA_NACK,	errorCondition,	I2CS_IDLE			},
	{I2CS_TRANSFERRING,	TW_MT_ARB_LOST,		errorCondition,	I2CS_IDLE			},
	
	{I2CS_TRANSFERRING,	TW_BUS_ERROR,		errorCondition,	I2CS_IDLE			},
};

static I2C_STATEMACHINE sm = {false, 0, I2CS_IDLE, sm_entries};

I2C_STATEMACHINE * I2C_MT_GetSM(void) {return &sm;}

void I2C_MT_Start(void)
{
	start();
}

static void sendAddress(void)
{
	uint8_t address = (data()->address << 1);
	TWDR = address;
	ack();
}

static void errorCondition(void)
{
	switch(sm.currentEvent)
	{
	case TW_MT_SLA_NACK:
	case TW_MT_DATA_NACK:
		stop();
		break;
	case TW_MT_ARB_LOST: // also TW_MR_ARB_LOST
		release();
		break;
	default:
		break;
	}
	
	I2C_Done(false);
}

static void txNextByte(void)
{
	if (!I2C_BufferUsed())
	{
		TWDR = data()->buffer[data()->bytesTransferred++]; // More data to send
		ack();
	}
	else
	{
		stop(); // No more data to send
		I2C_Done(true);
	}
}

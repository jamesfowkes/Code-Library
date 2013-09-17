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
static void sendFirstAck(void);
static void getNextByte(void);
static void finish(void);
static void errorCondition(void);

static I2C_STATEMACHINEENTRY sm_entries[] = 
{
	{I2CS_IDLE,			TW_START,			sendAddress,	I2CS_ADDRESSING		},
	{I2CS_IDLE,			TW_REP_START,		sendAddress,	I2CS_ADDRESSING		},
	{I2CS_IDLE,			TW_BUS_ERROR,		errorCondition,	I2CS_IDLE			},
	
	{I2CS_ADDRESSING,	TW_MR_SLA_ACK,		sendFirstAck,	I2CS_TRANSFERRING	},
	{I2CS_ADDRESSING,	TW_MR_SLA_NACK,		errorCondition,	I2CS_IDLE			},
	
	{I2CS_ADDRESSING,	TW_BUS_ERROR,		errorCondition,	I2CS_IDLE			},
	
	{I2CS_TRANSFERRING,	TW_MR_DATA_ACK,		getNextByte,	I2CS_TRANSFERRING	},
	{I2CS_TRANSFERRING,	TW_MR_DATA_NACK,	finish,			I2CS_TRANSFERRING	},
	{I2CS_TRANSFERRING,	TW_MR_ARB_LOST,		errorCondition,	I2CS_IDLE			},
	
	{I2CS_TRANSFERRING,	TW_BUS_ERROR,		errorCondition,	I2CS_IDLE			},
};

static I2C_STATEMACHINE sm = {false, 0, I2CS_IDLE, sm_entries};

I2C_STATEMACHINE * I2C_MR_GetSM(void) {return &sm;}

void I2C_MR_Start(void)
{
	start();
}

static void sendAddress(void)
{
	uint8_t address = (data()->address << 1);
	address |= 0x01; // For reading
	TWDR = address;
	ack();
}

static void sendFirstAck(void)
{
	if (!I2C_BufferFull()) { ack(); } else { nack(); }
}

static void getNextByte(void)
{
	data()->buffer[data()->bytesTransferred++] = TWDR;
	// More bytes to receive
	if (!I2C_BufferFull()) { ack(); } else { nack(); }
}

static void finish(void)
{
	data()->buffer[data()->bytesTransferred++] = TWDR;
	// No more bytes to receive after this
	stop();
	I2C_Done(true);
}

static void errorCondition(void)
{
	stop();
	I2C_Done(false);
}
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

#include "avr/io.h"
#include "util/twi.h"

/*
 * Common and Generic Includes
 */
#include "lib_i2c_common.h"
#include "lib_i2c_private.h"
#include "lib_i2c_defs.h"

#include "lib_io.h"

static void sendAddress(void);
static void errorCondition(void);
static void txNextByte(void);
static void finish(void);

static const I2C_STATEMACHINEENTRY sm_entries[] =
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
	{I2CS_TRANSFERRING, TW_REP_START,		finish,			I2CS_IDLE			},

	{I2CS_TRANSFERRING,	TW_BUS_ERROR,		errorCondition,	I2CS_IDLE			},
};

static I2C_STATEMACHINE sm = {false, 0, I2CS_IDLE, sm_entries};
static I2C_TRANSFER_DATA * pTransfer = NULL;

I2C_STATEMACHINE * I2C_MT_GetSM(void) {return &sm;}

static bool s_repeatStart = false;

void I2C_MT_SetTransferData(I2C_TRANSFER_DATA * transfer)
{
	pTransfer = transfer;
	pTransfer->bytesTransferred = 0;
}

void I2C_MT_Start(void)
{
	start();
}

void I2C_MT_SetRepeatStart(bool repeatStart)
{
	s_repeatStart = repeatStart;
}

static void sendAddress(void)
{
	uint8_t address = (pTransfer->address << 1);
	TWDR = address;
	send();
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
	if (!I2C_TxBufferUsed())
	{
		TWDR = pTransfer->buffer[pTransfer->bytesTransferred]; // More data to send
		(pTransfer->bytesTransferred)++;
		send();
	}
	else
	{
		if (s_repeatStart)
		{
			s_repeatStart = false;
			start();
		}
		else
		{
			stop(); // No more data to send
			I2C_Done(true);
		}
	}
}

static void finish(void)
{
	I2C_Done(true);
}

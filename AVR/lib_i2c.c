/*
 * Standard Library Includes
 */
#include <stdbool.h>
#include <stddef.h>

#include <stdlib.h>
#include <assert.h>

/*
 * Common and Generic Includes
 */
#include "lib_i2c_common.h"

/*
 * Private Typedefs
 */
enum i2c_state_enum
{
	I2CS_IDLE,
	I2CS_REQ,
	I2CS_STARTED,
	I2CS_ADDRESSING,
	I2CS_TRANSFERRING,
}
typedef enum i2c_state_enum I2C_STATE_ENUM;

enum i2c_event_enum
{
	I2CE_REQ,
	I2CE_START,
	I2CE_SLA_ACK,
	I2CE_DATA_ACK,
	I2CE_EOD
};
typedef enum i2c_event_enum I2C_EVENT_ENUM;

enum i2c_mode_enum
{
	I2CM_MT,
	I2CM_MR,
	I2CM_ST,
	I2CM_SR
};
typedef enum i2c_mode_enum I2C_MODE_ENUM;

typedef void (*i2c_sm_func)(void);

struct i2c_statemachineentry_struct
{
	I2C_STATE_ENUM	state;
	I2C_EVENT_ENUM	event;
	i2c_sm_func		handler;
	I2C_STATE_ENUM	nextState;
};
typedef struct i2c_statemachineentry_struct I2C_STATEMACHINEENTRY;

static I2C_MODE_ENUM mode = I2CM_MT;
static I2C_STATE_ENUM state = I2C_IDLE;
static I2C_TRANSFER_DATA currentTransferData;

static uint8_t byteCount = 0;

static I2C_STATEMACHINEENTRY sm[] = 
{
	{I2CS_IDLE,			I2CE_MT_REQ,	sendStart,		I2CS_REQ			},
	
	{I2CS_REQ,			I2CS_START,		sendAddress,	I2CS_ADDRESSING		},
	
	{I2CS_ADDRESSING	I2CS_SLA_ACK	txrxNextByte,	I2CS_TRANSFERRING	},
	
	{I2CS_TRANSFERRING,	I2CE_DATA_ACK	txrxNextByte,	I2CS_TRANSFERRING	},
	{I2CS_TRANSFERRING,	I2CE_ERROR		errorCondition,	I2CS_IDLE			},
	{I2CS_TRANSFERRING,	I2CE_EOD,		finishTxRx,		I2CS_IDLE			},
}

bool I2C_Init(void)
{
	return true;
}

bool I2C_MasterWrite(I2C_TRANSFER_DATA * transferData)
{
	bool success = true;
	if (transferData)
	{
		if(transferData.buffer && transferData.byteCount)
		{
			currentTransferData = transferData;
			byteCount = 0;
			mode = I2CM_MT;
			twi_sm_event(I2CE_MT_REQ);
		}
	}
	return success;
}

bool I2C_MasterRead(I2C_TRANSFER_DATA * transferData)
{
	bool success = true;
	if (transferData)
	{
		if(transferData.buffer && transferData.byteCount)
		{
			currentTransferData = transferData;
			byteCount = 0;
			mode = I2CM_MR;
			twi_sm_event(I2CE_MR_REQ);
		}
	}
	return success;
}

void sendStart(void)
{
	TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
}

void sendAddress(void)
{
	address = (currentTransferData.address << 1);
	address |= (mode == I2CM_MR) ? 0x01 : 0x00;
	
	TWDR = address;
	TWCR = (1<<TWINT) | (1<<TWEN);
}

void txrxNextByte(void)
{
	if (byteCount < (transferData.bytes - 1))
	{
		switch (mode)
		{
		case I2CM_MT:
			TWDR = transferData.buffer[byteCount++];
			TWCR = (1<<TWINT) | (1<<TWEN);
			break;
		case I2CM_MR:
			transferData.buffer[byteCount++] = TWDR;
			if (byteCount < (transferData.bytes - 1))
			{
				ack();
			}
			else
			{
				nack()
			}
			break;
		case I2CM_ST:
			break;
		case I2CM_SR:
			break;
		}
		
		byteCount++;
	}
	else
	{
		i2c_sm_event(I2CE_EOD);
	}
}

void errorCondition(void)
{
	stop(void);
	transferData.callback(false);
}

void finishTxRx(void)
{
	stop(void);
	transferData.callback(true);
}

void ack(void)
{
	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWEA);
}

void nack(void)
{
	TWCR = (1<<TWINT) | (1<<TWEN);
}

void stop(void)
{
	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO);
}

ISR(TWI_vect)
{
	uint8_t status = TWSR;
	if (status & TWINT)
	{
		switch(status & 0xF8)
		{
		case TW_START:
			i2c_sm_event(I2CE_START);
			break;
		case TW_MT_SLA_ACK:
			i2c_sm_event(I2CE_SLA_ACK);
			break;
		case TW_MT_DATA_ACK:
		case TW_MR_DATA_ACK:
			i2c_sm_event(I2CE_DATA_ACK);
			break;
		default:
			i2c_sm_event(I2CE_ERROR);
		}
	}
}
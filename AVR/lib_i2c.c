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

/*
 * Private Typedefs
 */
enum i2c_state_enum
{
	I2CS_IDLE,
	I2CS_STARTING,
	I2CS_ADDRESSING,
	I2CS_TRANSFERRING,
};
typedef enum i2c_state_enum I2C_STATE_ENUM;

enum i2c_mode_enum
{
	I2CM_NONE,
	I2CM_MT,
	I2CM_MR,
	I2CM_ST,
	I2CM_SR
};
typedef enum i2c_mode_enum I2C_MODE_ENUM;

typedef void (*i2c_sm_func)(uint8_t);

struct i2c_statemachineentry_struct
{
	I2C_STATE_ENUM	currentState;
	uint8_t			event;
	i2c_sm_func		handler;
	I2C_STATE_ENUM	nextState;
};
typedef struct i2c_statemachineentry_struct I2C_STATEMACHINEENTRY;

struct i2c_statemachine_struct
{
	bool busy;
	I2C_STATEMACHINEENTRY* entries;
};
typedef struct i2c_statemachine_struct I2C_STATEMACHINE;

static bool startMaster(I2C_TRANSFER_DATA * newTransferData, I2C_MODE_ENUM selected_mode);

static void startSR(uint8_t event);
static void startST(uint8_t event);

static void errorCondition(uint8_t event);
static void sendAddress(uint8_t event);
static void txNextByte(uint8_t event);
static void rxDATA(uint8_t event);
static void rxSLA(uint8_t event);

static void sendAck(uint8_t event);
static void sendNack(uint8_t event);
static void ack_or_nack_on_data_length(void);

static void ack();
static void nack();
static void stop();
static void release();

static void finishTx();
static void finishRx(uint8_t event);

static bool bufferFull(void);

static void i2c_sm_event(uint8_t event);

static I2C_STATEMACHINEENTRY sm_emtries[] = 
{
	{I2CS_IDLE,			TW_ST_SLA_ACK,				startST,		I2CS_TRANSFERRING	},
	{I2CS_IDLE,			TW_ST_ARB_LOST_SLA_ACK,		startST,		I2CS_TRANSFERRING	},
		
	{I2CS_IDLE,			TW_SR_SLA_ACK,				startSR,		I2CS_TRANSFERRING	},
	{I2CS_IDLE,			TW_SR_GCALL_ACK,			startSR,		I2CS_TRANSFERRING	},
	{I2CS_IDLE,			TW_SR_ARB_LOST_SLA_ACK,		startSR,		I2CS_TRANSFERRING	},
	{I2CS_IDLE,			TW_SR_ARB_LOST_GCALL_ACK,	startSR,		I2CS_TRANSFERRING	},
	
	{I2CS_IDLE,			TW_START,					sendAddress,	I2CS_ADDRESSING		},
	{I2CS_IDLE,			TW_BUS_ERROR,				errorCondition,	I2CS_IDLE			},
		
	{I2CS_IDLE,			TW_BUS_ERROR,				errorCondition,	I2CS_IDLE			},
				
	{I2CS_ADDRESSING,	TW_MT_SLA_ACK,				txNextByte,		I2CS_TRANSFERRING	},
	{I2CS_ADDRESSING,	TW_MT_SLA_NACK,				errorCondition,	I2CS_IDLE			},
			
	{I2CS_ADDRESSING,	TW_MR_SLA_ACK,				rxSLA,			I2CS_TRANSFERRING	},
	{I2CS_ADDRESSING,	TW_MR_SLA_NACK,				rxSLA,			I2CS_TRANSFERRING	},
			
	{I2CS_ADDRESSING,	TW_BUS_ERROR,				errorCondition,	I2CS_IDLE			},
			
	{I2CS_TRANSFERRING,	TW_MT_DATA_ACK,				txNextByte,		I2CS_TRANSFERRING	},
	{I2CS_TRANSFERRING,	TW_MT_DATA_NACK,			errorCondition,	I2CS_IDLE			},
	{I2CS_TRANSFERRING,	TW_MT_ARB_LOST,				errorCondition,	I2CS_IDLE			},
			
	{I2CS_TRANSFERRING,	TW_MR_DATA_ACK,				rxDATA,			I2CS_TRANSFERRING	},
	{I2CS_TRANSFERRING,	TW_MR_DATA_NACK,			rxDATA,			I2CS_TRANSFERRING	},
	{I2CS_TRANSFERRING,	TW_MR_ARB_LOST,				errorCondition,	I2CS_IDLE			},
			
	{I2CS_TRANSFERRING,	TW_ST_DATA_ACK,				txNextByte,		I2CS_TRANSFERRING	},
	{I2CS_TRANSFERRING,	TW_ST_DATA_NACK,			sendAck,		I2CS_IDLE			},
	{I2CS_TRANSFERRING,	TW_ST_LAST_DATA,			sendAck,		I2CS_IDLE			},
		
	{I2CS_TRANSFERRING,	TW_SR_DATA_ACK,				rxSLA,			I2CS_TRANSFERRING	},
	{I2CS_TRANSFERRING,	TW_SR_GCALL_DATA_ACK,		rxSLA,			I2CS_TRANSFERRING	},
	{I2CS_TRANSFERRING,	TW_SR_DATA_NACK,			sendNack,		I2CS_TRANSFERRING	},
	{I2CS_TRANSFERRING,	TW_SR_GCALL_DATA_NACK,		sendNack,		I2CS_TRANSFERRING	},
	{I2CS_TRANSFERRING,	TW_SR_STOP,					finishRx,		I2CS_IDLE			},
		
	{I2CS_TRANSFERRING,	TW_BUS_ERROR,				errorCondition,	I2CS_IDLE			},
};

static I2C_STATEMACHINE sm = {false, sm_emtries};

static I2C_MODE_ENUM mode = I2CM_NONE;
static I2C_STATE_ENUM state = I2CS_IDLE;

static I2C_CONDITION_ENUM transferCondition = I2CC_CONTINUE;

static I2C_TRANSFER_DATA * transferData;

static I2C_SLAVE_HANDLER slaveRxHandler;
static I2C_SLAVE_HANDLER slaveTxHandler;

static uint8_t byteCount = 0;

bool I2C_Init(I2C_SLAVE_HANDLER srxHandler, I2C_SLAVE_HANDLER stxHandler)
{
	slaveRxHandler = srxHandler;
	slaveTxHandler = stxHandler;
	return true;
}

bool I2C_MasterWrite(I2C_TRANSFER_DATA * newTransferData)
{
	return startMaster(newTransferData, I2CM_MT);
}

bool I2C_MasterRead(I2C_TRANSFER_DATA * newTransferData)
{
	return startMaster(newTransferData, I2CM_MT);
}

static bool startMaster(I2C_TRANSFER_DATA * newTransferData, I2C_MODE_ENUM selected_mode)
{
	bool success = true;
	if (newTransferData)
	{
		if(newTransferData->buffer && newTransferData->byteCount)
		{
			// Start in MT or MR mode. First response will be TW_START
			transferData = newTransferData;
			transferCondition = I2CC_CONTINUE;
			mode = selected_mode;
			byteCount = 0;
			TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
		}
	}
	return success;

}

static void startST(uint8_t event)
{
	(void)event;
	mode = I2CM_ST;
	transferCondition = I2CC_CONTINUE;
	byteCount = 0;
	
	transferData->buffer = NULL;
	transferData->byteCount = 0;
	slaveTxHandler(transferData);
	
	// Act like we got a data ACK
	txNextByte(TW_ST_DATA_ACK);
}

static void startSR(uint8_t event)
{
	(void)event;
	mode = I2CM_SR;
	transferCondition = I2CC_CONTINUE;
	byteCount = 0;
	ack();
}

static void sendAddress(uint8_t event)
{
	(void)event;
	uint8_t address = (transferData->address << 1);
	address |= (mode == I2CM_MR) ? 0x01 : 0x00;
	
	TWDR = address;
	ack();
}

static void txNextByte(uint8_t event)
{
	(void)event;
	if (!bufferFull())
	{
		TWDR = transferData->buffer[byteCount++]; // More data to send
		ack();
	}
	else
	{
		finishTx(); // No more data to send
	}
}

static void rxDATA(uint8_t event)
{
	switch (mode)
	{
	case I2CM_MR:
		// If this is a data ack/nack, store the next byte
		switch (event)
		{
		case TW_MR_DATA_ACK:
			transferData->buffer[byteCount++] = TWDR;
			ack_or_nack_on_data_length();
			break;
		case TW_MR_DATA_NACK:
			transferData->buffer[byteCount++] = TWDR;
			errorCondition(TW_MR_DATA_NACK);
			break;
		default:
			break;
		}
	case I2CM_SR:
		// Store the data, then ack/nack
		transferData->buffer[byteCount++] = TWDR;
		ack_or_nack_on_data_length();
		break;
	default:
		break;
	}
}

static void rxSLA(uint8_t event)
{
	switch (mode)
	{
	case I2CM_MR:
		// If this is a data ack/nack, store the next byte
		switch (event)
		{
		case TW_MR_SLA_ACK:
			ack_or_nack_on_data_length();
			break;
		case TW_MR_SLA_NACK:
			stop();
			break;
		default:
			break;
		}
	case I2CM_SR:
		// Store the data, then ack/nack
		transferData->buffer[byteCount++] = TWDR;
		ack_or_nack_on_data_length();
		break;
	default:
		break;
	}
}

static void sendAck(uint8_t event)
{
	(void)event;
	ack();
}

static void sendNack(uint8_t event)
{
	(void)event;
	nack();
}

static void ack_or_nack_on_data_length(void)
{
	if (!bufferFull())
	{
		ack(); //ACK if more bytes to come
	}
	else
	{
		nack();
	}
}

static bool bufferFull(void)
{
	return (byteCount == (transferData->byteCount - 1));
}

static void finishTx(void)
{
	switch(mode)
	{
	case I2CM_MT:
		stop();
		break;
	case I2CM_ST:
		ack_or_nack_on_data_length();
		break;
	default:
		break;
	}
	
	transferCondition = I2CC_SUCCESS;
}

static void finishRx(uint8_t event)
{
	(void)event;
	switch(mode)
	{
	case I2CM_MR:
		stop();
		break;
	case I2CM_SR:
		stop();
		release();
		break;
	default:
		break;
	}
	
	transferCondition = I2CC_SUCCESS;
}

static void errorCondition(uint8_t event)
{
	switch(event)
	{
	case TW_MT_SLA_NACK:
	case TW_MT_DATA_NACK:
	case TW_MR_SLA_NACK:
		stop();
		break;
	case TW_MT_ARB_LOST:
		release();
		break;
	default:
		break;
	}
	
	transferCondition = I2CC_FAILED;
}

static void ack(void)
{
	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWEA);
}

static void nack(void)
{
	TWCR = (1<<TWINT) | (1<<TWEN);
}

static void stop(void)
{
	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO);
	mode = I2CM_NONE;
}

static void release(void)
{
	TWCR = (1<<TWEN) | (1<<TWIE) | (1<<TWEA) | (1<<TWINT);
	mode = I2CM_NONE;
}

static void i2c_sm_event(uint8_t event)
{
	sm.busy = true;
	
	I2C_STATEMACHINEENTRY *search = sm.entries;
	
	// Find state
	while (search->currentState != state)
	{
		search++;
	}
	
	while (search->currentState == state)
	{
		if (search->event == event) {break;}
	}
	
	if (search->event == event)
	{
		search->handler(event);
		if (I2CC_CONTINUE == transferCondition)
		{
			state = search->nextState;
		}
		else
		{
			transferData->callback(transferCondition);
		}
	}
	
	sm.busy = false;
}

ISR(TWI_vect)
{
	uint8_t status = TWSR;
	if (status & TWINT)
	{
		status = status & 0xF8;
		
		if (status != TW_NO_INFO)
		{
			while (sm.busy) {continue;}
			i2c_sm_event(status);
		}
	}
}

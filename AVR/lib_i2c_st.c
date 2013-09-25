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

static void startST(void);
static void txNextByte(void);
static void finish(void);
static void errorCondition(void);

static I2C_STATEMACHINEENTRY sm_entries[] = 
{
	{I2CS_IDLE,			TW_ST_SLA_ACK,			startST,		I2CS_TRANSFERRING	},
	{I2CS_IDLE,			TW_ST_ARB_LOST_SLA_ACK,	startST,		I2CS_TRANSFERRING	},
	
	{I2CS_IDLE,			TW_BUS_ERROR,			errorCondition,	I2CS_IDLE			},
	
	{I2CS_TRANSFERRING,	TW_ST_DATA_ACK,			txNextByte,		I2CS_TRANSFERRING	},
	{I2CS_TRANSFERRING,	TW_ST_DATA_NACK,		finish,			I2CS_IDLE			},
	{I2CS_TRANSFERRING,	TW_ST_LAST_DATA,		finish,			I2CS_IDLE			},
	
	{I2CS_TRANSFERRING,	TW_BUS_ERROR,			errorCondition,	I2CS_IDLE			},
};

static I2C_STATEMACHINE sm = {false, 0, I2CS_IDLE, sm_entries};

I2C_STATEMACHINE * I2C_ST_GetSM(void) {return &sm;}

static void startST(void)
{
	// Act like we got a data ACK
	txNextByte();
}

static void txNextByte(void)
{
	TWDR = data()->buffer[data()->bytesTransferred++]; // More data to send
	if (!I2C_BufferFull()) { ack(); } else { nack(); }
}

static void finish(void)
{
	ack();
	I2C_Done(true);
}

static void errorCondition(void)
{
	stop();
	I2C_Done(false);
}

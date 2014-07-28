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

#include "lib_io.h"
#include "lib_i2c_common.h"
#include "lib_i2c_private.h"
#include "lib_i2c_defs.h"

#include "util_macros.h"

static I2C_EVENT mapToSMEvent(uint8_t status);

static bool s_bTWINTSet = false;
static uint8_t s_lastTWSR;

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

void I2C_SetPrescaler(uint8_t divisor)
{
	TWBR = divisor;
}

static I2C_EVENT mapToSMEvent(uint8_t status)
{
	I2C_EVENT e = I2C_STOP;

	switch(status)
	{
	case TW_START:
		e = I2C_START;
		break;
	case TW_REP_START:
		e = I2C_REP_START;
		break;
	case TW_MT_SLA_ACK:
		e = I2C_SLA_ACK;
		break;
	case TW_MT_SLA_NACK:
		e = I2C_SLA_NACK;
		break;
	case TW_MT_DATA_ACK:
		e = I2C_DATA_ACK;
		break;
	case TW_MT_DATA_NACK:
		e = I2C_DATA_NACK;
		break;
	case TW_MT_ARB_LOST: // Also TW_MR_ARB_LOST
		e = I2C_ARB_LOST;
		break;
	case TW_MR_SLA_ACK:
		e = I2C_SLA_ACK;
		break;
	case TW_MR_SLA_NACK:
		e = I2C_SLA_NACK;
		break;
	case TW_MR_DATA_ACK:
		e = I2C_DATA_ACK;
		break;
	case TW_MR_DATA_NACK:
		e = I2C_DATA_NACK;
		break;
	case TW_ST_SLA_ACK:
		e = I2C_ST_SLA_ACK;
		break;
	case TW_ST_ARB_LOST_SLA_ACK:
		e = I2C_ARB_LOST_SLA_ACK;
		break;
	case TW_ST_DATA_ACK:
		e = I2C_DATA_ACK;
		break;
	case TW_ST_DATA_NACK:
		e = I2C_DATA_NACK;
		break;
	case TW_ST_LAST_DATA:
		e = I2C_DATA_NACK;
		break;
	case TW_SR_SLA_ACK:
		e = I2C_SR_SLA_ACK;
		break;
	case TW_SR_ARB_LOST_SLA_ACK:
		e = I2C_ARB_LOST_SLA_ACK;
		break;
	case TW_SR_GCALL_ACK:
		e = I2C_GCALL_ACK;
		break;
	case TW_SR_ARB_LOST_GCALL_ACK:
		e = I2C_ARB_LOST_GCALL_ACK;
		break;
	case TW_SR_DATA_ACK:
		e = I2C_DATA_ACK;
		break;
	case TW_SR_DATA_NACK:
		e = I2C_DATA_NACK;
		break;
	case TW_SR_GCALL_DATA_ACK:
		e = I2C_GCALL_DATA_ACK;
		break;
	case TW_SR_GCALL_DATA_NACK:
		e = I2C_GCALL_DATA_NACK;
		break;
	case TW_SR_STOP:
		e = I2C_STOP;
		break;
	case TW_NO_INFO:
		e = I2C_STOP;
		break;
	case TW_BUS_ERROR:
		e = I2C_BUS_ERROR;
		break;
	}	
	return e;
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
			I2C_New_Event( mapToSMEvent(status) );
		}
	}
}

ISR(TWI_vect)
{
	s_bTWINTSet = true;
	s_lastTWSR = TWSR;
}

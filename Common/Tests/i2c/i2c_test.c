#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

#include "unity.h"

#include "lib_i2c_common.h"

/*
 * Defines and Typedefs
 */


/* 
 * Private Function Prototypes
 */

static void rd_callback(I2C_TRANSFER_DATA * transfer);
static void wr_callback(I2C_TRANSFER_DATA * transfer);

/* 
 * Local Variables
 */

static I2C_TRANSFER_DATA s_i2c_transfer;
static uint8_t s_data[10];

static uint8_t s_count;

void setUp(void)
{
	I2C_Init(NULL);
	s_count = 0;
}

void tearDown(void)
{

}
	
void test_MR(void)
{
	s_i2c_transfer.buffer = s_data;
	s_i2c_transfer.totalBytes = sizeof(s_data);
	s_i2c_transfer.address = 0xAA;
	s_i2c_transfer.callback = wr_callback;
	I2C_StartMaster(&s_i2c_transfer, false, false);
}

static void rd_callback(I2C_TRANSFER_DATA * transfer)
{
	(void)transfer;
}

static void wr_callback(I2C_TRANSFER_DATA * transfer)
{
	(void)transfer;
}

void I2C_PrivateInit(void)
{

}

void start(void)
{
	I2C_NewEvent(I2CE_START);
}

void ack(void) {}

void send(void)
{
	if (s_bAddressSet)
	{
		I2C_NewEvent(I2CE_MR_SLA_ACK);
	}
	else
	{
		I2C_NewEvent(I2CE_MR_DATA_ACK);	
	}
}

void nack(void) {}
void stop(void)  {}
void release(void) {}

void I2C_PLATFORM_SET_ADDR(uint8_t address) { (void)address; }
void I2C_PLATFORM_GET_DATA(uint8_t data) { (void)data; }
void I2C_PLATFORM_SET_DATA(uint8_t data)
{
	(void)data;
	s_count++;
}


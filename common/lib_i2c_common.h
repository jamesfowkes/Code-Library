#ifndef _LIB_I2C_H_
#define _LIB_I2C_H_

enum i2c_event
{
	I2C_START,
	I2C_REP_START,
	I2C_BUS_ERROR,

	I2C_SLA_ACK,
	I2C_SLA_NACK,

	I2C_ST_SLA_ACK,
	I2C_SR_SLA_ACK,

	I2C_ARB_LOST_SLA_ACK,
	I2C_ARB_LOST_SLA_NACK,

	I2C_DATA_ACK,
	I2C_DATA_NACK,

	I2C_GCALL_ACK,
	I2C_ARB_LOST_GCALL_ACK,

	I2C_GCALL_DATA_ACK,
	I2C_GCALL_DATA_NACK,

	I2C_ARB_LOST,

	I2C_STOP
};
typedef enum i2c_event I2C_EVENT;

struct i2c_transfer_data_struct; // Pre-declare the struct so the callback can be declared

typedef void (*I2C_CALLBACK)(struct i2c_transfer_data_struct *);

struct i2c_transfer_data_struct
{
	uint8_t			address;
	uint8_t			*buffer;
	uint8_t			bytesTransferred;
	uint8_t			totalBytes;
	bool			success;
	I2C_CALLBACK	callback;
};
typedef struct i2c_transfer_data_struct I2C_TRANSFER_DATA;

typedef I2C_TRANSFER_DATA* (*I2C_SLAVE_HANDLER)(bool read);

bool I2C_Init(I2C_SLAVE_HANDLER slaveHandler);
bool I2C_IsActive(void);

void I2C_Task(void);

void I2C_SetSlaveAddress(uint8_t slaveAddress);

void I2C_AcceptGCALL(bool accept);

bool I2C_StartMaster(I2C_TRANSFER_DATA * newTransferData, bool read, bool repeatStart);
bool I2C_StartMasterFromRS(I2C_TRANSFER_DATA * newTransferData, bool read, bool repeatStart);

void I2C_New_Event(I2C_EVENT event);

#endif

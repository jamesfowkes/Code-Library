#ifndef _LIB_I2C_H_
#define _LIB_I2C_H_

typedef void (*I2CCALLBACK)(bool success);

struct i2c_transfer_data_struct
{
	uint8_t			address;
	uint8_t			*buffer;
	uint8_t			byteCount;
	I2CCALLBACK		callback;
};
typedef struct i2c_transfer_data_struct I2C_TRANSFER_DATA;

bool I2C_Init(void);

bool I2C_MasterWrite(I2C_TRANSFER_DATA * transfer);
bool I2C_MasterRead(I2C_TRANSFER_DATA * transfer);
bool I2C_SlaveWrite(I2C_TRANSFER_DATA * transfer);
bool I2C_SlaveRead(I2C_TRANSFER_DATA * transfer);

#endif

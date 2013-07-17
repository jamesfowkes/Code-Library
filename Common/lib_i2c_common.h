#ifndef _LIB_I2C_H_
#define _LIB_I2C_H_

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

bool I2C_Write(I2C_TRANSFER_DATA * transfer);
bool I2C_Read(I2C_TRANSFER_DATA * transfer);

#endif

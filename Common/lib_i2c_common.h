#ifndef _LIB_I2C_H_
#define _LIB_I2C_H_

typedef void (*I2C_CALLBACK)(bool success);

struct i2c_transfer_data_struct
{
	uint8_t			address;
	uint8_t			*buffer;
	uint8_t			byteCount;
	I2C_CALLBACK	callback;
};
typedef struct i2c_transfer_data_struct I2C_TRANSFER_DATA;

enum i2c_condition_enum
{
	I2CC_CONTINUE,
	I2CC_FAILED,
	I2CC_SUCCESS,
};
typedef enum i2c_condition_enum I2C_CONDITION_ENUM;

typedef void (*I2C_SLAVE_HANDLER)(I2C_TRANSFER_DATA * transfer);

bool I2C_Init(I2C_SLAVE_HANDLER srxHandler, I2C_SLAVE_HANDLER stxHandler);

bool I2C_MasterWrite(I2C_TRANSFER_DATA * transfer);
bool I2C_MasterRead(I2C_TRANSFER_DATA * transfer);

#endif

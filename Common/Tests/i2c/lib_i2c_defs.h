#ifndef _LIB_I2C_DEFS_H
#define _LIB_I2C_DEFS_H

void start(void);
void ack(void);
void send(void);
void nack(void);
void stop(void);
void release(void);

void I2C_PLATFORM_SET_ADDR(uint8_t address);
void I2C_PLATFORM_GET_DATA(uint8_t data);
void I2C_PLATFORM_SET_DATA(uint8_t data);

#endif

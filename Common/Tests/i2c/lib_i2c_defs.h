#ifndef _LIB_I2C_DEFS_H
#define _LIB_I2C_DEFS_H

void start(void);
void ack(void);
void send(void);
void nack(void);
void stop(void);
void release(void);

void setThisSlaveAddress(uint8_t address);
void readData(uint8_t * data);
void setData(uint8_t * data);

#endif

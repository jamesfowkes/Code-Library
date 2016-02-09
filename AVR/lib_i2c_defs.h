#ifndef _LIB_I2C_DEFS_H
#define _LIB_I2C_DEFS_H

#include "util/twi.h"

#define start()		{ TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN)  | (1<<TWIE); }
#define ack()		{ TWCR = (1<<TWINT) | (1<<TWEN)  | (1<<TWEA)  | (1<<TWIE); }
#define send()		{ TWCR = (1<<TWINT) | (1<<TWEN)  | (1<<TWIE); }
#define nack()		{ TWCR = (1<<TWINT) | (1<<TWEN)  | (1<<TWIE); }
#define stop()		{ TWCR = (1<<TWINT) | (1<<TWEN)  | (1<<TWSTO) | (1<<TWIE); }
#define release()	{ TWCR = (1<<TWEN)  | (1<<TWIE)  | (1<<TWEA)  | (1<<TWINT); }

inline void setData(uint8_t * x)	{ TWDR = *x; }
inline void readData(uint8_t * x) { *x = TWDR; }

#define setThisSlaveAddress(x) { TWAR &= 0x01; TWAR |= (x << 1); }

#endif

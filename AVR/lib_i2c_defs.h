#ifndef _LIB_I2C_DEFS_H
#define _LIB_I2C_DEFS_H

#define start()		{ TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN); }
#define ack()		{ TWCR = (1<<TWINT) | (1<<TWEN)  | (1<<TWEA); }
#define nack()		{ TWCR = (1<<TWINT) | (1<<TWEN); }
#define stop()		{ TWCR = (1<<TWINT) | (1<<TWEN)  | (1<<TWSTO); }
#define release()	{ TWCR = (1<<TWEN)  | (1<<TWIE)  | (1<<TWEA) | (1<<TWINT); }

#endif

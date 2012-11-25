#ifndef _LIB_IO_H_
#define _LIB_IO_H_

/*
 * Public Datatype Defintions
 */
enum io_state_enum
{
	IO_OFF,
	IO_ON,
};
typedef enum io_state_enum IO_STATE_ENUM;

void IO_Control(volatile uint8_t *port, IO_STATE_ENUM eState, uint8_t pin);

#endif

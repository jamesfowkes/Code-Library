#ifndef _LIB_IO_H_
#define _LIB_IO_H_

/*
 * Public Datatype Defintions
 */

enum io_port_enum
{
	IO_PORTA,
	IO_PORTB,
	IO_PORTC,
	IO_PORTD
};
typedef enum io_port_enum IO_PORT_ENUM;

enum io_state_enum
{
	IO_OFF,
	IO_ON,
	IO_TOGGLE
};
typedef enum io_state_enum IO_STATE_ENUM;

enum io_mode_enum
{
	IO_MODE_INPUT,
	IO_MODE_PULLUPINPUT,
	IO_MODE_OUTPUT,
};
typedef enum io_mode_enum IO_MODE_ENUM;

void IO_SetMode(IO_PORT_ENUM ePort, uint8_t pin, IO_MODE_ENUM eMode);
void IO_Control(IO_PORT_ENUM ePort, uint8_t pin, IO_STATE_ENUM eState);

#endif

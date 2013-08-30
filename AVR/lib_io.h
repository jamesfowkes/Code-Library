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

typedef void (*EXT_IO_FN)(bool);

#define IO_On(port, pin) (port |= (1 <<  pin))
#define IO_Off(port, pin) (port &= ~(1 <<  pin))

#define IO_Read(port, pin) (bool)((port & (1 << pin)) == (1 << pin))

void IO_SetMode(IO_PORT_ENUM ePort, uint8_t pin, IO_MODE_ENUM eMode);
void IO_Toggle(IO_PORT_ENUM ePort, uint8_t pin);

#endif

#ifndef _LIB_IO_H_
#define _LIB_IO_H_

/*
 * Public Datatype Defintions
 */

enum io_port_enum
{
	#ifdef PORTA
	IO_PORTA,
	#endif
	#ifdef PORTB
	IO_PORTB,
	#endif
	#ifdef PORTC
	IO_PORTC,
	#endif
	#ifdef PORTD
	IO_PORTD
	#endif
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
#define IO_Toggle(pins, pin) (pins |= (1 << pin))
#define IO_Read(port, pin) (bool)((port & (1 << pin)) == (1 << pin))

void IO_Control(IO_PORT_ENUM ePort, uint8_t pin, IO_STATE_ENUM state);
void IO_SetMode(IO_PORT_ENUM ePort, uint8_t pin, IO_MODE_ENUM eMode);

#endif

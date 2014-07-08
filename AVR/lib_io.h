#ifndef _LIB_IO_H_
#define _LIB_IO_H_

#define SFRP(SFR) (uint8_t *)(&SFR)

/*
 * Public Datatype Defintions
 */

enum io_port_enum
{
	#if defined(PORTA0) || defined(TEST_HARNESS_PORTA)
	IO_PORTA,
	#endif
	#if defined(PORTB0) || defined(TEST_HARNESS_PORTB)
	IO_PORTB,
	#endif
	#if defined(PORTC0) || defined(TEST_HARNESS_PORTC)
	IO_PORTC,
	#endif
	#if defined(PORTD0) || defined(TEST_HARNESS_PORTD)
	IO_PORTD,
	#endif
	IO_PORT_MAX
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
	IO_MODE_I2C_PULLUP
};
typedef enum io_mode_enum IO_MODE_ENUM;

typedef void (*EXT_IO_FN)(bool);

#define IO_Output(ddr, pin) (ddr |= (1 << pin))
#define IO_Input(ddr, pin) (ddr &= ~(1 << pin))

#define IO_On(port, pin) (port |= (1 <<  pin))
#define IO_Off(port, pin) (port &= ~(1 <<  pin))
#define IO_Toggle(pins, pin) (pins |= (1 << pin))
#define IO_Read(port, pin) (bool)((port & (1 << pin)) == (1 << pin))

void IO_Control(IO_PORT_ENUM ePort, uint8_t pin, IO_STATE_ENUM state);
void IO_SetMode(IO_PORT_ENUM ePort, uint8_t pin, IO_MODE_ENUM eMode);
bool IO_ReadPin(IO_PORT_ENUM ePort, uint8_t pin);
uint8_t IO_ReadPort(IO_PORT_ENUM ePort);

volatile uint8_t * IO_GetWritePortDirect(IO_PORT_ENUM ePort);
volatile uint8_t * IO_GetReadPortDirect(IO_PORT_ENUM ePort);
volatile uint8_t * IO_GetDirectionPortDirect(IO_PORT_ENUM ePort);

#ifdef TEST_HARNESS
void IO_SetInput(IO_PORT_ENUM ePort, uint8_t pin, IO_STATE_ENUM state);
#endif

#endif

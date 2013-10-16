/*
 * Standard Library Includes
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/*
 * AVR Includes (Defines and Primitives)
 */

#include "avr/io.h"

/*
 * AVR Library Includes
 */

#include "lib_io.h"

/*
 * Private Function Prototypes
 */

void SetDDR(IO_PORT_ENUM ePort, uint8_t pin);
void ClearDDR(IO_PORT_ENUM ePort, uint8_t pin);
void SetPORT(IO_PORT_ENUM ePort, uint8_t pin);
void ClearPORT(IO_PORT_ENUM ePort, uint8_t pin);
void SetPINS(IO_PORT_ENUM ePort, uint8_t pin);
void ClearPINS(IO_PORT_ENUM ePort, uint8_t pin);

/*
 * Public Functions
 */

void IO_Control(IO_PORT_ENUM ePort, uint8_t pin, IO_STATE_ENUM state)
{
	switch(state)
	{
	case IO_OFF:
		ClearPORT(ePort, pin);
		break;
	case IO_ON:
		SetPORT(ePort, pin);
		break;
	case IO_TOGGLE:
		SetPINS(ePort, pin);
		break;
	}
}

void IO_SetMode(IO_PORT_ENUM ePort, uint8_t pin, IO_MODE_ENUM eMode)
{

	switch(eMode)
	{
	case IO_MODE_INPUT:
		ClearDDR(ePort, pin);
		ClearPORT(ePort, pin);
		break;
	case IO_MODE_PULLUPINPUT:
		ClearDDR(ePort, pin);
		SetPORT(ePort, pin);
		break;
	case IO_MODE_OUTPUT:
		SetDDR(ePort, pin);
		break;
	case IO_MODE_I2C_PULLUP:
		SetPORT(ePort, pin);
		break;
	}
}

/*
 * Private Functions
 */

void SetDDR(IO_PORT_ENUM ePort, uint8_t pin)
{

	switch(ePort)
	{
#ifdef DDRA
	case IO_PORTA:
		DDRA |= (1 << pin);
		break;
#endif
#ifdef DDRB
	case IO_PORTB:
		DDRB |= (1 << pin);
		break;
#endif
#ifdef DDRC
	case IO_PORTC:
		DDRC |= (1 << pin);
		break;
#endif
#ifdef DDRD
	case IO_PORTD:
		DDRD |= (1 << pin);
	break;
#endif
	}
}

void ClearDDR(IO_PORT_ENUM ePort, uint8_t pin)
{

	switch(ePort)
	{
#ifdef DDRA
	case IO_PORTA:
		DDRA &= ~(1 << pin);
		break;
#endif
#ifdef DDRB
	case IO_PORTB:
		DDRB &= ~(1 << pin);
		break;
#endif
#ifdef DDRC
	case IO_PORTC:
		DDRC &= ~(1 << pin);
		break;
#endif
#ifdef DDRD
	case IO_PORTD:
		DDRD &= ~(1 << pin);
		break;
#endif
	}
}

void SetPORT(IO_PORT_ENUM ePort, uint8_t pin)
{

	switch(ePort)
	{
#ifdef PORTA
	case IO_PORTA:
		PORTA |= (1 << pin);
		break;
#endif
#ifdef PORTB
	case IO_PORTB:
		PORTB |= (1 << pin);
		break;
#endif
#ifdef DDRC
	case IO_PORTC:
		PORTC |= (1 << pin);
		break;
#endif
#ifdef DDRD
	case IO_PORTD:
		PORTD |= (1 << pin);
	break;
#endif
	}
}

void ClearPORT(IO_PORT_ENUM ePort, uint8_t pin)
{
	switch(ePort)
	{
#ifdef PORTA
	case IO_PORTA:
		PORTA &= ~(1 << pin);
		break;
#endif
#ifdef PORTB
	case IO_PORTB:
		PORTB &= ~(1 << pin);
		break;
#endif
#ifdef DDRC
	case IO_PORTC:
		PORTC &= ~(1 << pin);
		break;
#endif
#ifdef DDRD
	case IO_PORTD:
		PORTD &= ~(1 << pin);
	break;
#endif
	}
}

void SetPINS(IO_PORT_ENUM ePort, uint8_t pin)
{

	switch(ePort)
	{
#ifdef PINA
	case IO_PORTA:
		PINA = (1 << pin);
		break;
#endif
#ifdef PINB
	case IO_PORTB:
		PINB = (1 << pin);
		break;
#endif
#ifdef DDRC
	case IO_PORTC:
		PINC = (1 << pin);
		break;
#endif
#ifdef DDRD
	case IO_PORTD:
		PIND = (1 << pin);
	break;
#endif
	}
}

void ClearPINS(IO_PORT_ENUM ePort, uint8_t pin)
{

	switch(ePort)
	{
#ifdef PINA
	case IO_PORTA:
		PINA &= ~(1 << pin);
		break;
#endif
#ifdef PINB
	case IO_PORTB:
		PINB &= ~(1 << pin);
		break;
#endif
#ifdef DDRC
	case IO_PORTC:
		PINC &= ~(1 << pin);
		break;
#endif
#ifdef DDRD
	case IO_PORTD:
		PIND &= ~(1 << pin);
	break;
#endif
	}
}

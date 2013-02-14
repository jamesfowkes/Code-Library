/*
 * Standard Library Includes
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/*
 * AVR Includes (Defines and Primitives)
 */
#ifdef __AVR_ATtiny84A__
#error "TINY"
#endif
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
	}
}

void IO_Control(IO_PORT_ENUM ePort, uint8_t pin, IO_STATE_ENUM eState)
{
	switch(eState)
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

/*
 * Private Functions
 */

void SetDDR(IO_PORT_ENUM ePort, uint8_t pin)
{

	switch(ePort)
	{
	case IO_PORTA:
		#ifdef DDRA
		DDRA |= (1 << pin);
		#endif
		break;
	case IO_PORTB:
		#ifdef DDRB
		DDRB |= (1 << pin);
		#endif
		break;
	case IO_PORTC:
		#ifdef DDRC
		DDRC |= (1 << pin);
		#endif
		break;
	case IO_PORTD:
		#ifdef DDRD
		DDRD |= (1 << pin);
		#endif
		break;
	}
}

void ClearDDR(IO_PORT_ENUM ePort, uint8_t pin)
{

	switch(ePort)
	{
	case IO_PORTA:
		#ifdef DDRA
		DDRA &= ~(1 << pin);
		#endif
		break;
	case IO_PORTB:
		#ifdef DDRB
		DDRB &= ~(1 << pin);
		#endif
		break;
	case IO_PORTC:
		#ifdef DDRC
		DDRC &= ~(1 << pin);
		#endif
		break;
	case IO_PORTD:
		#ifdef DDRD
		DDRD &= ~(1 << pin);
		#endif
		break;
	}
}

void SetPORT(IO_PORT_ENUM ePort, uint8_t pin)
{

	switch(ePort)
	{
	case IO_PORTA:
		#ifdef PORTA
		PORTA |= (1 << pin);
		#endif
		break;
	case IO_PORTB:
		#ifdef PORTB
		PORTB |= (1 << pin);
		#endif
		break;
	case IO_PORTC:
		#ifdef PORTC
		PORTC |= (1 << pin);
		#endif
		break;
	case IO_PORTD:
		#ifdef PORTD
		PORTD |= (1 << pin);
		#endif
		break;
	}
}

void ClearPORT(IO_PORT_ENUM ePort, uint8_t pin)
{

	switch(ePort)
	{
	case IO_PORTA:
		#ifdef PORTA
		PORTA &= ~(1 << pin);
		#endif
		break;
	case IO_PORTB:
		#ifdef PORTB
		PORTB &= ~(1 << pin);
		#endif
		break;
	case IO_PORTC:
		#ifdef PORTC
		PORTC &= ~(1 << pin);
		#endif
		break;
	case IO_PORTD:
		#ifdef PORTD
		PORTD &= ~(1 << pin);
		#endif
		break;
	}
}

void SetPINS(IO_PORT_ENUM ePort, uint8_t pin)
{

	switch(ePort)
	{
	case IO_PORTA:
		#ifdef PINA
		PINA |= (1 << pin);
		#endif
		break;
	case IO_PORTB:
		#ifdef PINB
		PINB |= (1 << pin);
		#endif
		break;
	case IO_PORTC:
		#ifdef PINC
		PINC |= (1 << pin);
		#endif
		break;
	case IO_PORTD:
		#ifdef PIND
		PIND |= (1 << pin);
		#endif
		break;
	}
}

void ClearPINS(IO_PORT_ENUM ePort, uint8_t pin)
{

	switch(ePort)
	{
	case IO_PORTA:
		#ifdef PINA
		PINA &= ~(1 << pin);
		#endif
		break;
	case IO_PORTB:
		#ifdef PINB
		PINB &= ~(1 << pin);
		#endif
		break;
	case IO_PORTC:
		#ifdef PINC
		PINC &= ~(1 << pin);
		#endif
		break;
	case IO_PORTD:
		#ifdef PIND
		PIND &= ~(1 << pin);
		#endif
		break;
	}
}

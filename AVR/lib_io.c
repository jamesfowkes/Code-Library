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

bool IO_ReadPin(IO_PORT_ENUM ePort, uint8_t pin)
{
	bool on = false;
	
	switch(ePort)
	{
#if defined(PORTA0) || defined(TEST_HARNESS_PORTA)
	case IO_PORTA:
		on = IO_Read(PINA, pin);
		break;
#endif
#if defined(PORTB0) || defined(TEST_HARNESS_PORTB)
	case IO_PORTB:
		on = IO_Read(PINB, pin);
		break;
#endif
#if defined(PORTC0) || defined(TEST_HARNESS_PORTC)
	case IO_PORTC:
		on = IO_Read(PINC, pin);
		break;
#endif
#if defined(PORTD0) || defined(TEST_HARNESS_PORTD)
	case IO_PORTD:
		on = IO_Read(PIND, pin);
	break;
#endif
	default:
		break;
	}
	
	return on;
}

uint8_t IO_ReadPort(IO_PORT_ENUM ePort)
{
	uint8_t val = 0x00;
	
	switch(ePort)
	{
#if defined(PORTA0) || defined(TEST_HARNESS_PORTA)
	case IO_PORTA:
		val = PINA;
		break;
#endif
#if defined(PORTB0) || defined(TEST_HARNESS_PORTB)
	case IO_PORTB:
		val = PINB;
		break;
#endif
#if defined(PORTC0) || defined(TEST_HARNESS_PORTC)
	case IO_PORTC:
		val = PINC;
		break;
#endif
#if defined(PORTD0) || defined(TEST_HARNESS_PORTD)
	case IO_PORTD:
		val = PIND;
	break;
#endif
	default:
		break;
	}
	
	return val;
}

volatile uint8_t * IO_GetWritePortDirect(IO_PORT_ENUM ePort)
{

	volatile uint8_t * port = NULL;

	switch(ePort)
	{
#if defined(PORTA0) || defined(TEST_HARNESS_PORTA)
	case IO_PORTA:
		port = &PORTA;
		break;
#endif
#if defined(PORTB0) || defined(TEST_HARNESS_PORTB)
	case IO_PORTB:
		port = &PORTB;
		break;
#endif
#if defined(PORTC0) || defined(TEST_HARNESS_PORTC)
	case IO_PORTC:
		port = &PORTC;
		break;
#endif
#if defined(PORTD0) || defined(TEST_HARNESS_PORTD)
	case IO_PORTD:
		port = &PORTD;
	break;
#endif
	default:
		break;
	}

	return port;
}

volatile uint8_t * IO_GetReadPortDirect(IO_PORT_ENUM ePort)
{

	volatile uint8_t * port = NULL;

	switch(ePort)
	{
#if defined(PORTA0) || defined(TEST_HARNESS_PORTA)
	case IO_PORTA:
		port = &PINA;
		break;
#endif
#if defined(PORTB0) || defined(TEST_HARNESS_PORTB)
	case IO_PORTB:
		port = &PINB;
		break;
#endif
#if defined(PORTC0) || defined(TEST_HARNESS_PORTC)
	case IO_PORTC:
		port = &PINC;
		break;
#endif
#if defined(PORTD0) || defined(TEST_HARNESS_PORTD)
	case IO_PORTD:
		port = &PIND;
	break;
#endif
	default:
		break;
	}

	return port;
}

volatile uint8_t * IO_GetDirectionPortDirect(IO_PORT_ENUM ePort)
{

	volatile uint8_t * port = NULL;

	switch(ePort)
	{
#if defined(DDA0) || defined(TEST_HARNESS_DDRA)
	case IO_PORTA:
		port = &DDRA;
		break;
#endif
#if defined(DDB0) || defined(TEST_HARNESS_DDRB)
	case IO_PORTB:
		port = &DDRB;
		break;
#endif
#if defined(DDC0) || defined(TEST_HARNESS_DDRC)
	case IO_PORTC:
		port = &DDRC;
		break;
#endif
#if defined(DDD0) || defined(TEST_HARNESS_DDRD)
	case IO_PORTD:
		port = &DDRD;
	break;
#endif
	default:
		break;
	}

	return port;
}

#ifdef TEST_HARNESS
void IO_SetInput(IO_PORT_ENUM ePort, uint8_t pin, IO_STATE_ENUM state)
{
	volatile uint8_t * reg = IO_GetReadPortDirect(ePort);
	
	switch (state)
	{
	case IO_ON:
		*reg |= (1 << pin);
		break;
	case IO_OFF:
		*reg &= ~(1 << pin);
		break;
	case IO_TOGGLE:
		*reg ^= (1 << pin);
		break;
	default:
		break;
	}
}

#endif

/*
 * Private Functions
 */

void SetDDR(IO_PORT_ENUM ePort, uint8_t pin)
{

	switch(ePort)
	{
#if defined(DDA0) || defined(TEST_HARNESS_DDRA)
	case IO_PORTA:
		DDRA |= (1 << pin);
		break;
#endif
#if defined(DDB0) || defined(TEST_HARNESS_DDRB)
	case IO_PORTB:
		DDRB |= (1 << pin);
		break;
#endif
#if defined(DDC0) || defined(TEST_HARNESS_DDRC)
	case IO_PORTC:
		DDRC |= (1 << pin);
		break;
#endif
#if defined(DDD0) || defined(TEST_HARNESS_DDRD)
	case IO_PORTD:
		DDRD |= (1 << pin);
	break;
#endif
	default:
		break;
	}
}

void ClearDDR(IO_PORT_ENUM ePort, uint8_t pin)
{

	switch(ePort)
	{
#if defined(DDA0) || defined(TEST_HARNESS_DDRA)
	case IO_PORTA:
		DDRA &= ~(1 << pin);
		break;
#endif
#if defined(DDB0) || defined(TEST_HARNESS_DDRB)
	case IO_PORTB:
		DDRB &= ~(1 << pin);
		break;
#endif
#if defined(DDC0) || defined(TEST_HARNESS_DDRC)
	case IO_PORTC:
		DDRC &= ~(1 << pin);
		break;
#endif
#if defined(DDD0) || defined(TEST_HARNESS_DDRD)
	case IO_PORTD:
		DDRD &= ~(1 << pin);
		break;
#endif
	default:
		break;
	}
}

void SetPORT(IO_PORT_ENUM ePort, uint8_t pin)
{

	switch(ePort)
	{
#if defined(PORTA0) || defined(TEST_HARNESS_PORTA)
	case IO_PORTA:
		PORTA |= (1 << pin);
		break;
#endif
#if defined(PORTB0) || defined(TEST_HARNESS_PORTB)
	case IO_PORTB:
		PORTB |= (1 << pin);
		break;
#endif
#if defined(PORTC0) || defined(TEST_HARNESS_PORTC)
	case IO_PORTC:
		PORTC |= (1 << pin);
		break;
#endif
#if defined(PORTD0) || defined(TEST_HARNESS_PORTD)
	case IO_PORTD:
		PORTD |= (1 << pin);
	break;
#endif
	default:
		break;
	}
}

void ClearPORT(IO_PORT_ENUM ePort, uint8_t pin)
{
	switch(ePort)
	{
#if defined(PORTA0) || defined(TEST_HARNESS_PORTA)
	case IO_PORTA:
		PORTA &= ~(1 << pin);
		break;
#endif
#if defined(PORTB0) || defined(TEST_HARNESS_PORTB)
	case IO_PORTB:
		PORTB &= ~(1 << pin);
		break;
#endif
#if defined(PORTC0) || defined(TEST_HARNESS_PORTC)
	case IO_PORTC:
		PORTC &= ~(1 << pin);
		break;
#endif
#if defined(PORTD0) || defined(TEST_HARNESS_PORTD)
	case IO_PORTD:
		PORTD &= ~(1 << pin);
	break;
#endif
	default:
		break;
	}
}

void SetPINS(IO_PORT_ENUM ePort, uint8_t pin)
{

	switch(ePort)
	{
#if defined(PINA0) || defined(TEST_HARNESS_PINA)
	case IO_PORTA:
		PINA = (1 << pin);
		break;
#endif
#if defined(PINB0) || defined(TEST_HARNESS_PINB)
	case IO_PORTB:
		PINB = (1 << pin);
		break;
#endif
#if defined(PINC0) || defined(TEST_HARNESS_PINC)
	case IO_PORTC:
		PINC = (1 << pin);
		break;
#endif
#if defined(PIND0) || defined(TEST_HARNESS_PIND)
	case IO_PORTD:
		PIND = (1 << pin);
	break;
#endif
	default:
		break;
	}
}

void ClearPINS(IO_PORT_ENUM ePort, uint8_t pin)
{

	switch(ePort)
	{
#if defined(PINA0) || defined(TEST_HARNESS_PINA)
	case IO_PORTA:
		PINA &= ~(1 << pin);
		break;
#endif
#if defined(PINB0) || defined(TEST_HARNESS_PINB)
	case IO_PORTB:
		PINB &= ~(1 << pin);
		break;
#endif
#if defined(PINC0) || defined(TEST_HARNESS_PINC)
	case IO_PORTC:
		PINC &= ~(1 << pin);
		break;
#endif
#if defined(PIND0) || defined(TEST_HARNESS_PIND)
	case IO_PORTD:
		PIND &= ~(1 << pin);
	break;
#endif
	default:
		break;
	}
}

/*************************************************************************
Title:    Interrupt UART library with receive/transmit circular buffers
Author:   Peter Fleury <pfleury@gmx.ch>   http://jump.to/fleury
File:     $Id: uart.c,v 1.10 2013/06/02 07:27:04 peter Exp $
Software: AVR-GCC 4.1, AVR Libc 1.4.6 or higher
Hardware: any AVR with built-in UART, 
License:  GNU General Public License 
		
DESCRIPTION:
	An interrupt is generated when the UART has finished transmitting or
	receiving a byte. The interrupt handling routines use circular buffers
	for buffering received and transmitted data.
	
	The UART_RX_BUFFER_SIZE and UART_TX_BUFFER_SIZE variables define
	the buffer size in bytes. Note that these variables must be a 
	power of 2.
	
USAGE:
	Refere to the header file uart.h for a description of the routines. 
	See also example test_uart.c.

NOTES:
	Based on Atmel Application Note AVR306
					
LICENSE:
	Copyright (C) 2006 Peter Fleury

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.
						
*************************************************************************/

/*
 * Standard Library Includes
 */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/*
 * AVR Includes (Defines and Primitives)
 */

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>

/*
 * AVR Library Includes
 */

#include "lib_clk.h"
#include "lib_uart.h"

/*
 * Generic Library Includes
 */

#include "ringbuf.h"
#include "memorypool.h"

/*
 *  Private Defines and Typedefs
 */

/** @brief  UART Baudrate Expression
 *  @param  xtalcpu  system clock in Mhz, e.g. 4000000UL for 4Mhz          
 *  @param  baudrate baudrate in bps, e.g. 1200, 2400, 9600     
 */
#define UART_BAUD_SELECT(baudRate,xtalCpu)  (((xtalCpu) + 8UL * (baudRate)) / (16UL * (baudRate)) -1UL)

/** @brief  UART Baudrate Expression for ATmega double speed mode
 *  @param  xtalcpu  system clock in Mhz, e.g. 4000000UL for 4Mhz           
 *  @param  baudrate baudrate in bps, e.g. 1200, 2400, 9600     
 */
#define UART_BAUD_SELECT_DOUBLE_SPEED(baudRate,xtalCpu) ( ((((xtalCpu) + 4UL * (baudRate)) / (8UL * (baudRate)) -1UL)) | 0x8000)

#if defined(__AVR_AT90S2313__) \
	|| defined(__AVR_AT90S4414__) || defined(__AVR_AT90S4434__) \
	|| defined(__AVR_AT90S8515__) || defined(__AVR_AT90S8535__) \
	|| defined(__AVR_ATmega103__)
/* old AVR classic or ATmega103 with one UART */
#define AT90_UART
#define UART0_RECEIVE_INTERRUPT   UART_RX_vect 
#define UART0_TRANSMIT_INTERRUPT  UART_UDRE_vect
#define UART0_STATUS   USR
#define UART0_CONTROL  UCR
#define UART0_DATA     UDR  
#define UART0_UDRIE    UDRIE
#elif defined(__AVR_AT90S2333__) || defined(__AVR_AT90S4433__)
/* old AVR classic with one UART */
#define AT90_UART
#define UART0_RECEIVE_INTERRUPT   UART_RX_vect 
#define UART0_TRANSMIT_INTERRUPT  UART_UDRE_vect
#define UART0_STATUS   UCSRA
#define UART0_CONTROL  UCSRB
#define UART0_DATA     UDR 
#define UART0_UDRIE    UDRIE
#elif  defined(__AVR_ATmega8__) || defined(__AVR_ATmega16__) || defined(__AVR_ATmega32__) \
	|| defined(__AVR_ATmega323__)
/* ATmega with one USART */
#define ATMEGA_USART
#define UART0_RECEIVE_INTERRUPT   USART_RXC_vect
#define UART0_TRANSMIT_INTERRUPT  USART_UDRE_vect
#define UART0_STATUS   UCSRA
#define UART0_CONTROL  UCSRB
#define UART0_DATA     UDR
#define UART0_UDRIE    UDRIE
#elif defined (__AVR_ATmega8515__) || defined(__AVR_ATmega8535__)
#define ATMEGA_USART
#define UART0_RECEIVE_INTERRUPT   USART_RX_vect
#define UART0_TRANSMIT_INTERRUPT  USART_UDRE_vect
#define UART0_STATUS   UCSRA
#define UART0_CONTROL  UCSRB
#define UART0_DATA     UDR
#define UART0_UDRIE    UDRIE
#elif defined(__AVR_ATmega163__)
/* ATmega163 with one UART */
#define ATMEGA_UART
#define UART0_RECEIVE_INTERRUPT   UART_RX_vect
#define UART0_TRANSMIT_INTERRUPT  UART_UDRE_vect
#define UART0_STATUS   UCSRA
#define UART0_CONTROL  UCSRB
#define UART0_DATA     UDR
#define UART0_UDRIE    UDRIE
#elif defined(__AVR_ATmega162__) 
/* ATmega with two USART */
#define ATMEGA_USART0
#define ATMEGA_USART1
#define UART0_RECEIVE_INTERRUPT   USART0_RXC_vect
#define UART1_RECEIVE_INTERRUPT   USART1_RXC_vect
#define UART0_TRANSMIT_INTERRUPT  USART0_UDRE_vect
#define UART1_TRANSMIT_INTERRUPT  USART1_UDRE_vect
#define UART0_STATUS   UCSR0A
#define UART0_CONTROL  UCSR0B
#define UART0_DATA     UDR0
#define UART0_UDRIE    UDRIE0
#define UART1_STATUS   UCSR1A
#define UART1_CONTROL  UCSR1B
#define UART1_DATA     UDR1
#define UART1_UDRIE    UDRIE1
#elif defined(__AVR_ATmega64__) || defined(__AVR_ATmega128__) 
/* ATmega with two USART */
#define ATMEGA_USART0
#define ATMEGA_USART1
#define UART0_RECEIVE_INTERRUPT   USART0_RX_vect
#define UART1_RECEIVE_INTERRUPT   USART1_RX_vect
#define UART0_TRANSMIT_INTERRUPT  USART0_UDRE_vect
#define UART1_TRANSMIT_INTERRUPT  USART1_UDRE_vect
#define UART0_STATUS   UCSR0A
#define UART0_CONTROL  UCSR0B
#define UART0_DATA     UDR0
#define UART0_UDRIE    UDRIE0
#define UART1_STATUS   UCSR1A
#define UART1_CONTROL  UCSR1B
#define UART1_DATA     UDR1
#define UART1_UDRIE    UDRIE1
#elif defined(__AVR_ATmega161__)
/* ATmega with UART */
#error "AVR ATmega161 currently not supported by this libaray !"
#elif defined(__AVR_ATmega169__) 
/* ATmega with one USART */
#define ATMEGA_USART
#define UART0_RECEIVE_INTERRUPT   USART0_RX_vect
#define UART0_TRANSMIT_INTERRUPT  USART0_UDRE_vect
#define UART0_STATUS   UCSRA
#define UART0_CONTROL  UCSRB
#define UART0_DATA     UDR
#define UART0_UDRIE    UDRIE
#elif defined(__AVR_ATmega48__) || defined(__AVR_ATmega88__) || defined(__AVR_ATmega168__) || defined(__AVR_ATmega48P__) || defined(__AVR_ATmega88P__) || defined(__AVR_ATmega168P__) || defined(__AVR_ATmega328P__) \
	|| defined(__AVR_ATmega3250__) || defined(__AVR_ATmega3290__) ||defined(__AVR_ATmega6450__) || defined(__AVR_ATmega6490__)
/* ATmega with one USART */
#define ATMEGA_USART0
#define UART0_RECEIVE_INTERRUPT   USART_RX_vect
#define UART0_TRANSMIT_INTERRUPT  USART_UDRE_vect
#define UART0_STATUS   UCSR0A
#define UART0_CONTROL  UCSR0B
#define UART0_DATA     UDR0
#define UART0_UDRIE    UDRIE0
#elif defined(__AVR_ATtiny2313__) 
#define ATMEGA_USART
#define UART0_RECEIVE_INTERRUPT   USART_RX_vect
#define UART0_TRANSMIT_INTERRUPT  USART_UDRE_vect
#define UART0_STATUS   UCSRA
#define UART0_CONTROL  UCSRB
#define UART0_DATA     UDR
#define UART0_UDRIE    UDRIE
#elif defined(__AVR_ATmega329__) || \
	defined(__AVR_ATmega649__) || \
	defined(__AVR_ATmega325__) || \
	defined(__AVR_ATmega645__) 
/* ATmega with one USART */
#define ATMEGA_USART0
#define UART0_RECEIVE_INTERRUPT   USART0_RX_vect
#define UART0_TRANSMIT_INTERRUPT  USART0_UDRE_vect
#define UART0_STATUS   UCSR0A
#define UART0_CONTROL  UCSR0B
#define UART0_DATA     UDR0
#define UART0_UDRIE    UDRIE0
#elif defined(__AVR_ATmega2560__) || defined(__AVR_ATmega2561__) || defined(__AVR_ATmega1280__)  || defined(__AVR_ATmega1281__) || defined(__AVR_ATmega640__)
/* ATmega with two USART */
#define ATMEGA_USART0
#define ATMEGA_USART1
#define UART0_RECEIVE_INTERRUPT   USART0_RX_vect
#define UART1_RECEIVE_INTERRUPT   USART1_RX_vect
#define UART0_TRANSMIT_INTERRUPT  USART0_UDRE_vect
#define UART1_TRANSMIT_INTERRUPT  USART1_UDRE_vect
#define UART0_STATUS   UCSR0A
#define UART0_CONTROL  UCSR0B
#define UART0_DATA     UDR0
#define UART0_UDRIE    UDRIE0
#define UART1_STATUS   UCSR1A
#define UART1_CONTROL  UCSR1B
#define UART1_DATA     UDR1
#define UART1_UDRIE    UDRIE1  
#elif defined(__AVR_ATmega644__)
/* ATmega with one USART */
#define ATMEGA_USART0
#define UART0_RECEIVE_INTERRUPT   USART0_RX_vect
#define UART0_TRANSMIT_INTERRUPT  USART0_UDRE_vect
#define UART0_STATUS   UCSR0A
#define UART0_CONTROL  UCSR0B
#define UART0_DATA     UDR0
#define UART0_UDRIE    UDRIE0
#elif defined(__AVR_ATmega164P__) || defined(__AVR_ATmega324P__) || defined(__AVR_ATmega644P__)
/* ATmega with two USART */
#define ATMEGA_USART0
#define ATMEGA_USART1
#define UART0_RECEIVE_INTERRUPT   USART0_RX_vect
#define UART1_RECEIVE_INTERRUPT   USART1_RX_vect
#define UART0_TRANSMIT_INTERRUPT  USART0_UDRE_vect
#define UART1_TRANSMIT_INTERRUPT  USART1_UDRE_vect
#define UART0_STATUS   UCSR0A
#define UART0_CONTROL  UCSR0B
#define UART0_DATA     UDR0
#define UART0_UDRIE    UDRIE0
#define UART1_STATUS   UCSR1A
#define UART1_CONTROL  UCSR1B
#define UART1_DATA     UDR1
#define UART1_UDRIE    UDRIE1
#else
#error "no UART definition for MCU available"
#endif

#if defined( ATMEGA_USART1 )
#define NUMBER_OF_UARTS 2
#else
#define NUMBER_OF_UARTS 1
#endif

enum TXRX_ENUM
{
	TX,
	RX
};

/*
 *  Private Variables
*/

static volatile RING_BUFFER		s_buffers[NUMBER_OF_UARTS][2];
static volatile bool			s_flags[NUMBER_OF_UARTS][2];
static volatile uint8_t	s_lastRXError[NUMBER_OF_UARTS];

/*
 * Private Function Prototypes
 */

static void UART0_Init(uint16_t baudrate);
#if NUMBER_OF_UARTS == 2
static void UART1_Init(uint16_t baudrate);
#endif
static void EnableTXInterrupt(UART_ENUM eUART);
static void DisableTXInterrupt(UART_ENUM eUART);

ISR (UART0_RECEIVE_INTERRUPT)
/*************************************************************************
Function: UART Receive Complete interrupt
Purpose:  called when the UART has received a character
**************************************************************************/
{
	uint8_t data;
	uint8_t usr;
	uint8_t lastRxError;

	/* read UART status register and UART data register */ 
	usr  = UART0_STATUS;
	data = UART0_DATA;
	
	/* */
#if defined( AT90_UART )
	lastRxError = (usr & (_BV(FE)|_BV(DOR)) );
#elif defined( ATMEGA_USART )
	lastRxError = (usr & (_BV(FE)|_BV(DOR)) );
#elif defined( ATMEGA_USART0 )
	lastRxError = (usr & (_BV(FE0)|_BV(DOR0)) );
#elif defined ( ATMEGA_UART )
	lastRxError = (usr & (_BV(FE)|_BV(DOR)) );
#endif
	
	Ringbuf_Put((RING_BUFFER*)&s_buffers[UART0][RX], &data);
	s_flags[UART0][RX] = true;
	s_lastRXError[UART0] |= lastRxError;   
	
}

void UART0_Task(bool * rx, bool * tx)
{
	cli();
	
	*rx = s_flags[UART0][RX];
	*tx = s_flags[UART0][TX];
	s_flags[UART0][RX] = false;
	s_flags[UART0][TX] = false;
	
	sei();
}

#if NUMBER_OF_UARTS == 2
void UART1_Task(bool * rx, bool * tx)
{
	cli();
	
	*rx = s_flags[UART1][RX];
	*tx = s_flags[UART1][TX];
	s_flags[UART1][RX] = false;
	s_flags[UART1][TX] = false;
	
	sei();
}
#endif

ISR (UART0_TRANSMIT_INTERRUPT)
/*************************************************************************
Function: UART Data Register Empty interrupt
Purpose:  called when the UART is ready to transmit the next byte
**************************************************************************/
{
	uint8_t * pData = (uint8_t*)Ringbuf_Pop_Front((RING_BUFFER*)&s_buffers[UART0][TX]);
	
	if ( pData ) {
		UART0_DATA = *pData;  /* start transmission */
	}else{
		/* tx buffer empty, disable UDRE interrupt */
		DisableTXInterrupt(UART0);
		s_flags[UART0][TX] = true;
	}
}


/*************************************************************************
Function: UART_Init()
Purpose:  initialize UART and set baudrate
Input:    baudrate using macro UART_BAUD_SELECT()
Returns:  none
**************************************************************************/
bool UART_Init(UART_ENUM eUART, uint16_t baudrate, uint8_t txBufferSize, uint8_t rxBufferSize, bool use2X)
{

	bool success = false;
	
	// Get data buffers for this UART and initalise ringbuffer with them
	uint8_t * pDataBuffers[2];
	pDataBuffers[TX] = (uint8_t * )MEMPOOL_GetBytes(txBufferSize);
	pDataBuffers[RX] = (uint8_t * )MEMPOOL_GetBytes(rxBufferSize);
	
	Ringbuf_Init((RING_BUFFER*)&s_buffers[eUART][TX], pDataBuffers[TX], sizeof(uint8_t), txBufferSize, true);
	Ringbuf_Init((RING_BUFFER*)&s_buffers[eUART][RX], pDataBuffers[RX], sizeof(uint8_t), rxBufferSize, true);
	
	// Convert standard baudrate to register bitmap
	uint32_t fcpu = CLK_GetFcpu();
	if (use2X)
	{
		baudrate = UART_BAUD_SELECT_DOUBLE_SPEED(baudrate,fcpu);
	}
	else
	{
		baudrate = UART_BAUD_SELECT(baudrate,fcpu);
	}
	
	if (pDataBuffers[TX] && pDataBuffers[RX])
	{
		if (eUART == UART0)
		{
			Ringbuf_Init((RING_BUFFER*)&s_buffers[UART0][TX], pDataBuffers[TX], sizeof(uint8_t), txBufferSize, true);
			Ringbuf_Init((RING_BUFFER*)&s_buffers[UART0][RX], pDataBuffers[RX], sizeof(uint8_t), rxBufferSize, true);
			UART0_Init(baudrate);
			success = true;
		}
		#if NUMBER_OF_UARTS == 2
		else if (eUART == UART1)
		{
			Ringbuf_Init((RING_BUFFER*)&s_buffers[UART1][TX], pDataBuffers[TX], sizeof(uint8_t), txBufferSize, true);
			Ringbuf_Init((RING_BUFFER*)&s_buffers[UART1][RX], pDataBuffers[RX], sizeof(uint8_t), rxBufferSize, true);
			UART1_Init(baudrate);
			success = true;
		}
		#endif
	}
	
	return success;
}

static void UART0_Init(uint16_t baudrate)
{
#if defined( AT90_UART )
	/* set baud rate */
	UBRR = (uint8_t)baudrate; 

	/* enable UART receiver and transmmitter and receive complete interrupt */
	UART0_CONTROL = _BV(RXCIE)|_BV(RXEN)|_BV(TXEN);

#elif defined (ATMEGA_USART)
	/* Set baud rate */
	if ( baudrate & 0x8000 )
	{
		UART0_STATUS = (1<<U2X);  //Enable 2x speed 
		baudrate &= ~0x8000;
	}
	UBRRH = (uint8_t)(baudrate>>8);
	UBRRL = (uint8_t) baudrate;

	/* Enable USART receiver and transmitter and receive complete interrupt */
	UART0_CONTROL = _BV(RXCIE)|(1<<RXEN)|(1<<TXEN);
	
	/* Set frame format: asynchronous, 8data, no parity, 1stop bit */
	#ifdef URSEL
	UCSRC = (1<<URSEL)|(3<<UCSZ0);
	#else
	UCSRC = (3<<UCSZ0);
	#endif 
	
#elif defined (ATMEGA_USART0 )
	/* Set baud rate */
	if ( baudrate & 0x8000 ) 
	{
		UART0_STATUS = (1<<U2X0);  //Enable 2x speed 
		baudrate &= ~0x8000;
	}
	UBRR0H = (uint8_t)(baudrate>>8);
	UBRR0L = (uint8_t) baudrate;

	/* Enable USART receiver and transmitter and receive complete interrupt */
	UART0_CONTROL = _BV(RXCIE0)|(1<<RXEN0)|(1<<TXEN0);
	
	/* Set frame format: asynchronous, 8data, no parity, 1stop bit */
	#ifdef URSEL0
	UCSR0C = (1<<URSEL0)|(3<<UCSZ00);
	#else
	UCSR0C = (3<<UCSZ00);
	#endif 

#elif defined ( ATMEGA_UART )
	/* set baud rate */
	if ( baudrate & 0x8000 ) 
	{
		UART0_STATUS = (1<<U2X);  //Enable 2x speed 
		baudrate &= ~0x8000;
	}
	UBRRHI = (uint8_t)(baudrate>>8);
	UBRR   = (uint8_t) baudrate;

	/* Enable UART receiver and transmitter and receive complete interrupt */
	UART0_CONTROL = _BV(RXCIE)|(1<<RXEN)|(1<<TXEN);

#endif

}

/*************************************************************************
Function: UART_GetChar()
Purpose:  return byte from ringbuffer  
Returns:  lower byte:  received byte from ringbuffer
		higher byte: last receive error
**************************************************************************/
uint8_t UART_GetChar(UART_ENUM eUART, uint8_t * pRXError)
{
	#if NUMBER_OF_UARTS == 1
	if (eUART == UART1) {return 0;}// Only one UART, early return
	#endif
	
	uint8_t data = 0;
	uint8_t *pData = Ringbuf_Pop_Front( (RING_BUFFER*)&s_buffers[eUART][RX] );
	
	if (pData)
	{
		data = *pData;
	}
	
	if (pRXError)
	{
		*pRXError = s_lastRXError[eUART];
		s_lastRXError[eUART] = 0;
	}
	
	return data;
}

/*************************************************************************
Function: UART_PutChar()
Purpose:  write char to ringbuffer for transmitting via UART
Input:    uart to tx on, char to be transmitted
Returns:  none          
**************************************************************************/
void UART_PutChar(UART_ENUM eUART, uint8_t c)
{
	#if NUMBER_OF_UARTS == 1
	if (eUART == UART0) // Only one UART, so only process character if for UART0
	#endif
	{
		Ringbuf_Put((RING_BUFFER*)&s_buffers[UART1][TX], &c);
		/* Enable UDRE interrupt (ISR will start TX) */
		EnableTXInterrupt(eUART);
	}
}

/*************************************************************************
Function: UART_PutStr()
Purpose:  write string to ringbuffer for transmitting via UART
Input:    uart to tx on, string to be transmitted
Returns:  none          
**************************************************************************/
void UART_PutStr(UART_ENUM eUART, const uint8_t * const str)
{
	#if NUMBER_OF_UARTS == 1
	if (eUART == UART0) // Only one UART, so only process character if for UART0
	#endif
	{
		uint8_t length = strlen( (const char *)str);
		UART_PutChars(eUART, str, length);
	}
}

/*************************************************************************
Function: UART_PutChars()
Purpose:  write multiple chars to ringbuffer for transmitting via UART
Input:    uart to tx on, pointer to array to be transmitted, length of array
Returns:  none          
**************************************************************************/
void UART_PutChars(UART_ENUM eUART, const uint8_t * const data, uint8_t n)
{
	uint8_t i;

	#if NUMBER_OF_UARTS == 1
	if (eUART == UART0) // Only one UART, so only process character if for UART0
	#endif
	{
		if (n)
		{
			for (i = 0; i < n; ++i)
			{
				Ringbuf_Put((RING_BUFFER*)&s_buffers[eUART][TX], (RINGBUF_DATA)&data[i]);
			}
			/* Enable UDRE interrupt (ISR will start TX) */
			EnableTXInterrupt(eUART);
		}
	}
}

/*************************************************************************
Function: uart1_puts_p()
Purpose:  transmit string from program memory to UART1
Input:    program memory string to be transmitted
Returns:  none
**************************************************************************/
void UART_PutProgStr(UART_ENUM eUART, uint8_t * data)
{
	uint8_t c;
	uint8_t * count = 0;
	#if NUMBER_OF_UARTS == 1
	if (eUART == UART0) // Only one UART, so only process character if for UART0
	#endif
	{
		while ( (c = pgm_read_byte(data++)) )
		{
			Ringbuf_Put((RING_BUFFER*)&s_buffers[eUART][TX], &c);
			count++;
		}
		
		if (count)
		{
			EnableTXInterrupt(eUART);
		}
	}
}

static void EnableTXInterrupt(UART_ENUM eUART)
{
	if (eUART == UART0)
	{
		UART0_CONTROL |= _BV(UART0_UDRIE);
	}
	#if NUMBER_OF_UARTS == 2
	else
	{
		UART1_CONTROL |= _BV(UART1_UDRIE);
	}
	#endif
}

static void DisableTXInterrupt(UART_ENUM eUART)
{
	if (eUART == UART0)
	{
		UART0_CONTROL |= _BV(UART0_UDRIE);
	}
	#if NUMBER_OF_UARTS == 2
	else
	{
		UART1_CONTROL |= _BV(UART1_UDRIE);
	}
	#endif
}

/*
* these functions are only for ATmegas with two USART
*/

#if NUMBER_OF_UARTS == 2
/*************************************************************************
Function: uart1_init()
Purpose:  initialize UART1 and set baudrate
Input:    baudrate using macro UART_BAUD_SELECT()
Returns:  none
**************************************************************************/
static void UART1_Init(unsigned int baudrate)
{
	/* Set baud rate */
	if ( baudrate & 0x8000 ) 
	{
		UART1_STATUS = (1<<U2X1);  //Enable 2x speed 
		baudrate &= ~0x8000;
	}
	UBRR1H = (uint8_t)(baudrate>>8);
	UBRR1L = (uint8_t) baudrate;

	/* Enable USART receiver and transmitter and receive complete interrupt */
	UART1_CONTROL = _BV(RXCIE1)|(1<<RXEN1)|(1<<TXEN1);
	
	/* Set frame format: asynchronous, 8data, no parity, 1stop bit */   
	#ifdef URSEL1
	UCSR1C = (1<<URSEL1)|(3<<UCSZ10);
	#else
	UCSR1C = (3<<UCSZ10);
	#endif 
}

ISR(UART1_RECEIVE_INTERRUPT)
/*************************************************************************
Function: UART1 Receive Complete interrupt
Purpose:  called when the UART1 has received a character
**************************************************************************/
{
	uint8_t usr;
	uint8_t data;
	uint8_t lastRxError;

	/* read UART status register and UART data register */ 
	usr  = UART1_STATUS;
	data = UART1_DATA;
	lastRxError = (usr & (_BV(FE1)|_BV(DOR1)) );
	
	Ringbuf_Put((RING_BUFFER*)&s_buffers[UART1][RX], &data);
	s_flags[UART1][RX] = true;
	s_lastRXError[UART1] |= lastRxError;
}


ISR(UART1_TRANSMIT_INTERRUPT)
/*************************************************************************
Function: UART1 Data Register Empty interrupt
Purpose:  called when the UART1 is ready to transmit the next byte
**************************************************************************/
{
	uint8_t * pData = (uint8_t*)Ringbuf_Pop_Front((RING_BUFFER*)&s_buffers[UART0][TX]);
	
	if ( pData ) {
		UART1_DATA = *pData;  /* start transmission */
	}else{
		/* tx buffer empty, disable UDRE interrupt */
		UART1_CONTROL &= ~_BV(UART1_UDRIE);
		s_flags[UART1][TX] = true;
	}
}
#endif

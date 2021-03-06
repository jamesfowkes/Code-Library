#ifndef UART_H
#define UART_H
/************************************************************************
Title:    Interrupt UART library with receive/transmit circular buffers
Author:   Peter Fleury <pfleury@gmx.ch>   http://jump.to/fleury
File:     $Id: uart.h,v 1.12 2012/11/19 19:52:27 peter Exp $
Software: AVR-GCC 4.1, AVR Libc 1.4
Hardware: any AVR with built-in UART, tested on AT90S8515 & ATmega8 at 4 Mhz
License:  GNU General Public License 
Usage:    see Doxygen manual

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
	
************************************************************************/

/** 
*  @defgroup pfleury_uart UART Library
*  @code #include <uart.h> @endcode
* 
*  @brief Interrupt UART library using the built-in UART with transmit and receive circular buffers. 
*
*  This library can be used to transmit and receive data through the built in UART. 
*
*  An interrupt is generated when the UART has finished transmitting or
*  receiving a byte. The interrupt handling routines use circular buffers
*  for buffering received and transmitted data.
*
*  @note Based on Atmel Application Note AVR306
*  @author Peter Fleury pfleury@gmx.ch  http://jump.to/fleury
*/

/**@{*/


#if (__GNUC__ * 100 + __GNUC_MINOR__) < 304
#error "This library requires AVR-GCC 3.4 or later, update to newer AVR-GCC compiler !"
#endif


/*
** constants and macros
*/

/* 
** high byte error return code of uart_getc()
*/
#define UART_FRAME_ERROR      0x1000              /* Framing Error by UART       */
#define UART_OVERRUN_ERROR    0x0800              /* Overrun condition by UART   */
#define UART_PARITY_ERROR     0x0400              /* Parity Error by UART        */ 
#define UART_BUFFER_OVERFLOW  0x0200              /* receive ringbuffer overflow */
#define UART_NO_DATA          0x0100              /* no receive data available   */

enum uart_enum
{
	UART0,
	UART1
};
typedef enum uart_enum UART_ENUM;

/*
** function prototypes
*/

/**
@brief   Initialize UART and set baudrate 
@param   baudrate Specify baudrate using macro UART_BAUD_SELECT()
@return  none
*/
bool UART_Init(UART_ENUM eUART, unsigned int baudrate, uint8_t txBufferSize, uint8_t rxBufferSize, bool use2X);

void UART0_Task(bool * rx, bool * tx);
void UART1_Task(bool * rx, bool * tx);

/**
*  @brief   Get received byte from ringbuffer
*
* Returns in the lower byte the received character and in the 
* higher byte the last receive error.
* UART_NO_DATA is returned when no data is available.
*
*  @param   void
*  @return  lower byte:  received byte from ringbuffer
*  @return  higher byte: last receive status
*           - \b 0 successfully received data from UART
*           - \b UART_NO_DATA           
*             <br>no receive data available
*           - \b UART_BUFFER_OVERFLOW   
*             <br>Receive ringbuffer overflow.
*             We are not reading the receive buffer fast enough, 
*             one or more received character have been dropped 
*           - \b UART_OVERRUN_ERROR     
*             <br>Overrun condition by UART.
*             A character already present in the UART UDR register was 
*             not read by the interrupt handler before the next character arrived,
*             one or more received characters have been dropped.
*           - \b UART_FRAME_ERROR       
*             <br>Framing Error by UART
*/
uint8_t UART_GetChar(UART_ENUM eUART, uint8_t * pRXError);

/**
*  @brief   Put byte to ringbuffer for transmitting via UART
*  @param   data byte to be transmitted
*  @return  none
*/
void UART_PutChar(UART_ENUM eUART, uint8_t c);

/**
*  @brief   Put string to ringbuffer for transmitting via UART
*
*  The string is buffered by the uart library in a circular buffer
*  and interrupt is triggered to start transmission.
* 
*  @param   s string to be transmitted
*  @return  none
*/
void UART_PutStr(UART_ENUM eUART, const uint8_t * str);
/**
*  @brief   Put array of chars to ringbuffer for transmitting via UART
*
*  The char array is buffered by the uart library in a circular buffer
*  and interrupt is triggered to start transmission.
* 
*  @param   s string to be transmitted
*  @return  none
*/
void UART_PutChars(UART_ENUM eUART, const uint8_t * data, uint8_t n);

/**
* @brief    Put string from program memory to ringbuffer for transmitting via UART.
*
* The string is buffered by the uart library in a circular buffer
* and interrupt is triggered to start transmission.
*
* @param    s program memory string to be transmitted
* @return   none
* @see      uart_puts_P
*/
void UART_PutProgStr(UART_ENUM eUART, uint8_t * data);

/**@}*/


#endif // UART_H 


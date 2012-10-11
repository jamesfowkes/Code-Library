/*
 * lib_usci.h
 *
 *  Created on: 6 Jul 2012
 *      Author: james
 *	Description: Header file for TI MSP430 USCI Peripheral
 */
#ifndef _USCI_H_
#define _USCI_H_

/*
 * Public Enumerations
 */

enum lib_usci_usci_enum
{
  USCI_A,
  USCI_B,
};
typedef enum lib_usci_usci_enum LIB_USCI_USCI_ENUM;

enum lib_usci_usart_baudrate_enum
{
  BAUDRATE_9600,
  BAUDRATE_19200,
  BAUDRATE_38400,
  BAUDRATE_57600,
  BAUDRATE_115200,
  BAUDRATE_230400,
  BAUDRATE_460800,
};
typedef enum lib_usci_usart_baudrate_enum LIB_USCI_USART_BAUDRATE;

/*
 * Public Functions
 */
bool    USCI_ConfigureUART(LIB_USCI_USCI_ENUM eUSCI, LIB_CLK_CLKSIG_ENUM eClkSig, LIB_USCI_USART_BAUDRATE eBaudRate);
bool    USCI_ConfigureSPI(LIB_USCI_USCI_ENUM eUSCI, LIB_CLK_CLKSIG_ENUM eClkSig);
bool    USCI_ConfigureI2C(LIB_USCI_USCI_ENUM eUSCI, LIB_CLK_CLKSIG_ENUM eClkSig);
bool    USCI_ConfigureIRDA(LIB_USCI_USCI_ENUM eUSCI, LIB_CLK_CLKSIG_ENUM eClkSig);

#endif

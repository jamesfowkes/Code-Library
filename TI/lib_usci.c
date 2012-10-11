/*
 * lib_usci.c
 *
 *  Created on: 6 Jul 2012
 *      Author: james
 *      Description: Library file for TI MSP430 USCI Peripheral
 *      Dependencies:
 *              Clock System (CS) library
 */
#include <stdbool.h>

#include "periph/usci.h"

#include "assert.h"
#include "lib_clk.h"
#include "lib_usci.h"

/*
 * Public Functions
 */

bool USCI_ConfigureUART(LIB_USCI_USCI_ENUM eUSCI, LIB_CLK_CLKSIG_ENUM eClkSig, LIB_USCI_USART_BAUDRATE eBaudRate)
{
  bool bSuccess = true;


  assert(bSuccess);
  return bSuccess;
}

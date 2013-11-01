/*
 * Standard Library Includes
 */

#include <stdint.h>
#include <stdbool.h>

/*
 * AVR Includes (Defines and Primitives)
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>

/*
 * Generic Library Includes
 */

/*
 * AVR Library Includes
 */
 
#include "lib_clk.h"
#include "lib_uart.h"

/*
 * Device Includes
 */


/*
 * Common Library Includes
 */


/*
 * Defines and Typedefs
 */
#define BAUD_RATE 9600
#define TX_BUFFER_SIZE 16
#define RX_BUFFER_SIZE 16

#define enableApplicationInterrupts() (GIMSK |= (MAINS_INT_MASK | CLK_INT_MASK))
#define disableApplicationInterrupts() (GIMSK &= ~(MAINS_INT_MASK | CLK_INT_MASK))

/*
 * Private Function Prototypes
 */

static void handleUARTTask(void);
/*
 * Private Variables
 */

int main(void)
{
	bool success = true;
	
	CLK_Init(0);
	
	success &= UART_Init(UART0, BAUD_RATE, TX_BUFFER_SIZE, RX_BUFFER_SIZE, false);

	while (!success) { continue; }
	
	while(1)
	{
		handleUARTTask();
	}
	
	return 0;
}

static void handleUARTTask(void)
{
	bool rxBufferHasData;
	bool txComplete;
	
	UART0_Task(&rxBufferHasData, &txComplete);
}
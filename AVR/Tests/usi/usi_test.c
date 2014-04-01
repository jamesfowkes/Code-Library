/*
 * Standard Library Includes
 */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

/*
 * AVR Library Includes (Harness)
 */

#include <avr/io.h>

#include "../../lib_usi.h"
#include "../../Harness/lib_usi_harness_functions.h"

int main(int argc, char *argv[])
{
	(void)argc;
	(void)argv;
	
	setbuf(stdout, NULL);
	
	USICR = 0;
	
	assert((USICR & 0x30) == 0x00);
	USI_SetMode(USI_MODE_THREE_WIRE);
	assert((USICR & (1 << USIWM0)) == (1 << USIWM0));

	assert((USICR & 0x40) == 0x00);
	USI_CounterInterruptEnable(true);
	assert((USICR & (1 << USIOIE)) == (1 << USIOIE));

	assert(USIDR == 0);
	USI_Harness_KickOverflowInMs(500);
	assert(USI_SendByte(0xAA) == 0xAA);
	assert(USIDR == 0xAA);
	assert((USICR & (1 << USITC)) == (1 << USITC));
	
	return 0;
}

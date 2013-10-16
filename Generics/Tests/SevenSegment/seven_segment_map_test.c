/*
 * Standard Library Includes
 */

#include <stdint.h>
#include <stdbool.h>

#include <stdio.h>

/*
 * Generic Library Includes
 */

#include "seven_segment_map.h"

static SEVEN_SEGMENT_MAP map = {
	0,
	1,
	2,
	3,
	4,
	5,
	6,
	7
};

static void printDigit(i, digit)
{
	printf("Digit %d maps to %d, which is:\n\n", i, digit);
	
	// Print line for A segment
	if (digit & (1 << map.A))
	{
		printf("#####");
	}
	printf("\n");
	
	// Print lines for B and F segments
	if ((digit & (1 << map.B)) && (digit & (1 << map.F)))
	{
		printf("#   #\n");
		printf("#   #\n");
	}
	else if (digit & (1 << map.B))
	{
		printf("    #\n");
		printf("    #\n");
	}
	else if (digit & (1 << map.F))
	{
		printf("#    \n");
		printf("#    \n");
	}
	
	// Print line for G segment
	if (digit & (1 << map.G))
	{
		printf("#####");
	}
	printf("\n");
	
	// Print lines for C and E segments
	if ((digit & (1 << map.C)) && (digit & (1 << map.E)))
	{
		printf("#   #\n");
		printf("#   #\n");
	}
	else if (digit & (1 << map.C))
	{
		printf("    #\n");
		printf("    #\n");
	}
	else if (digit & (1 << map.E))
	{
		printf("#    \n");
		printf("#    \n");
	}
	
	// Print line for D segment
	if (digit & (1 << map.D))
	{
		printf("#####");
	}
	printf("\n");
}

int main(void)
{
	uint8_t i = 0;
	uint8_t digit = 0;
	
	for (i = 0; i <= 0x0F; ++i)
	{
		digit = SSEG_CreateDigit(i, &map, true);
		printDigit(i, digit);
	}
}

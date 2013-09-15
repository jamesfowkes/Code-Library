/*
 * Standard Library Includes
 */

#include <stdint.h>
#include <stdbool.h>

/*
 * Generic Library Includes
 */

#include "seven_segment_map.h"

/*
 * Private Function Prototypes
 */
uint8_t A(uint8_t digit, uint8_t bit);
uint8_t B(uint8_t digit, uint8_t bit);
uint8_t C(uint8_t digit, uint8_t bit);
uint8_t D(uint8_t digit, uint8_t bit);
uint8_t E(uint8_t digit, uint8_t bit);
uint8_t F(uint8_t digit, uint8_t bit);
uint8_t G(uint8_t digit, uint8_t bit);

uint8_t SSEG_CreateDigit(uint8_t digit, SEVEN_SEGMENT_MAP * map, bool tails)
{
	uint8_t display = 0;
	
	display |= A(digit, map->A);
	display |= B(digit, map->B);
	display |= C(digit, map->C);
	display |= D(digit, map->D);
	display |= E(digit, map->E);
	display |= F(digit, map->F);
	display |= G(digit, map->G);
	
	if (tails)
	{
		if (digit == 6) { display |= map->A; }
		if (digit == 9) { display |= map->D; }
	}
	
	return display;
}

void SSEG_AddDecimal(uint8_t *pDisplay, SEVEN_SEGMENT_MAP * map)
{
	*pDisplay |= (1 << map->DP);
}

uint8_t A(uint8_t digit, uint8_t bit)
{
	return ((digit != 1) && (digit != 4) && (digit != 6)) ? (1 << bit) : 0;
}

uint8_t B(uint8_t digit, uint8_t bit)
{
	return ((digit != 5) && (digit != 6)) ? (1 << bit) : 0;
}

uint8_t C(uint8_t digit, uint8_t bit)
{
	return (digit != 2) ? (1 << bit) : 0;
}

uint8_t D(uint8_t digit, uint8_t bit)
{
	return ((digit != 1) && (digit != 4) && (digit != 7) && (digit != 9)) ? (1 << bit) : 0;
}

uint8_t E(uint8_t digit, uint8_t bit)
{
	return ((digit == 0) || (digit == 2) || (digit == 6) || (digit == 8)) ? (1 << bit) : 0;
}

uint8_t F(uint8_t digit, uint8_t bit)
{
	return ((digit != 1) && (digit != 2) && (digit != 3) && (digit != 7)) ? (1 << bit) : 0;
}

uint8_t G(uint8_t digit, uint8_t bit)
{
	return ((digit != 0) && (digit != 1) && (digit != 7)) ? (1 << bit) : 0;
}

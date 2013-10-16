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
	
	if (digit < 16)
	{
		display |= A(digit, map->A);
		display |= B(digit, map->B);
		display |= C(digit, map->C);
		display |= D(digit, map->D);
		display |= E(digit, map->E);
		display |= F(digit, map->F);
		display |= G(digit, map->G);
		
		if (tails)
		{
			if (digit == 6) { display |= (1 << map->A); }
			if (digit == 9) { display |= (1 << map->D); }
		}
	}
	
	return display;
}

void SSEG_AddDecimal(uint8_t *pDisplay, SEVEN_SEGMENT_MAP * map)
{
	*pDisplay |= (1 << map->DP);
}

uint8_t A(uint8_t digit, uint8_t bit)
{
						   //0, 1, 2, 3, 4, 5, 6, 7, 8, 9, A, B, C, D, E, F
	uint8_t segmentOn[16] =	{1, 0, 1, 1, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0, 1, 1};
	return segmentOn[digit] ? (1 << bit) : 0;
}

uint8_t B(uint8_t digit, uint8_t bit)
{
						   //0, 1, 2, 3, 4, 5, 6, 7, 8, 9, A, B, C, D, E, F
	uint8_t segmentOn[16] =	{1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 0, 1, 0, 0};
	return segmentOn[digit] ? (1 << bit) : 0;
}

uint8_t C(uint8_t digit, uint8_t bit)
{
						   //0, 1, 2, 3, 4, 5, 6, 7, 8, 9, A, B, C, D, E, F
	uint8_t segmentOn[16] =	{1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 0};
	return segmentOn[digit] ? (1 << bit) : 0;
}

uint8_t D(uint8_t digit, uint8_t bit)
{
						   //0, 1, 2, 3, 4, 5, 6, 7, 8, 9, A, B, C, D, E, F
	uint8_t segmentOn[16] =	{1, 0, 1, 1, 0, 1, 1, 0, 1, 0, 0, 1, 1, 1, 1, 0};
	return segmentOn[digit] ? (1 << bit) : 0;
}

uint8_t E(uint8_t digit, uint8_t bit)
{
						   //0, 1, 2, 3, 4, 5, 6, 7, 8, 9, A, B, C, D, E, F
	uint8_t segmentOn[16] =	{1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1};
	return segmentOn[digit] ? (1 << bit) : 0;
}

uint8_t F(uint8_t digit, uint8_t bit)
{
						   //0, 1, 2, 3, 4, 5, 6, 7, 8, 9, A, B, C, D, E, F
	uint8_t segmentOn[16] =	{1, 0, 0, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1};
	return segmentOn[digit] ? (1 << bit) : 0;
}

uint8_t G(uint8_t digit, uint8_t bit)
{
						   //0, 1, 2, 3, 4, 5, 6, 7, 8, 9, A, B, C, D, E, F
	uint8_t segmentOn[16] =	{0, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1};
	return segmentOn[digit] ? (1 << bit) : 0;
}

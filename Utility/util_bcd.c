#include <stdint.h>

#include "util_bcd.h"

uint8_t to_bcd(uint8_t from)
{
	uint8_t bcd = from / 10; // Get 10s char
	from-= (bcd * 10); // Subtract 10s from initial value to get units
	bcd <<= 4; //Shift up 10s value
	bcd += from; // Add on units

	return bcd;
}

uint8_t from_bcd(uint8_t bcd)
{
	uint8_t to = 0;

	// Extract 10s (mask, shift, multiply - keeps within 8-bit bounds)
	to = (bcd & 0xF0) >> 4;
	to *= 10;
	// Then add on units
	to += (bcd & 0x0F);

	return to;
}

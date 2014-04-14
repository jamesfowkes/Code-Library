/*
 * Standard library includes
 */

#include <stdint.h>

/*
 * Utility Library Includes
 */

#include "util_angle.h"
#include "util_macros.h"

/*
 * Public Functions
 */

uint16_t reciprocal_deg(int16_t a)
{
	a *= 10;
	a = reciprocal_tdeg(a);
	a = div_round_pos(a, 10);
	return (uint16_t)a;
}

uint16_t reciprocal_tdeg(int16_t a)
{
	a = a % 3600;
	if (a < 0)
	{
		a = 3600 + a;
	}
	return (a < 1800) ? (uint16_t)(a + 1800) : (uint16_t)(a - 1800);
}

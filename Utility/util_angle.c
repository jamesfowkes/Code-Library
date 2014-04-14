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
 * Private Function Prototypes
 */
 
static uint16_t sanitiseAngleTdeg(int16_t a);

/*
 * Public Functions
 */

uint16_t reciprocal_deg(int16_t a)
{
	a *= 10;
	uint16_t result = reciprocal_tdeg(a);
	return div_round_pos(result, 10);
}

uint16_t reciprocal_tdeg(int16_t a)
{
	uint16_t angle = sanitiseAngleTdeg(a);
	return (angle < 1800) ? (angle + 1800) : (angle - 1800);
}

uint16_t mirror_deg(int16_t a, int16_t mirror)
{
	uint16_t result = mirror_tdeg(a * 10, mirror * 10);
	return div_round_pos(result, 10);
}

uint16_t mirror_tdeg(int16_t a, int16_t mirror)
{
	uint16_t sanitised_a = sanitiseAngleTdeg(a);
	uint16_t sanitised_mirror = sanitiseAngleTdeg(mirror);
	
	return sanitiseAngleTdeg(3600 + sanitised_mirror - sanitised_a);
}

/*
 * Private Functions
 */
 
static uint16_t sanitiseAngleTdeg(int16_t a)
{
	while (a >= 3600) { a -= 3600; } // Bring below +360degrees
	while (a <0) { a += 3600; } // Bring above 0degrees
	return (uint16_t)a;
}

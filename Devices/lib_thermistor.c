/*
 * Standard library includes
 */

#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stddef.h>

/*
 * Utility Library Includes
 */

#include "fix16.h"

/*
 * Device Library Includes
 */

#include "lib_thermistor.h"

/*
 * Private defines and typedefs
 */

/*
 * Private Variables
 */

static fix16_t T25CinKelvin;
static fix16_t T0CinKelvin;

static fix16_t log2;

/*
 * Private Function Prototypes
 */
 
/*
 * Public Functions
 */

void THERMISTOR_Init(void)
{
	T25CinKelvin = fix16_div(fix16_from_int(29815), fix16_from_int(100));
	T0CinKelvin = fix16_div(fix16_from_int(27315), fix16_from_int(100));
	log2 = fix16_log(fix16_from_int(2));
}

bool THERMISTOR_InitDevice(THERMISTOR * pTherm, uint16_t B, uint32_t R25)
{
	bool success = false;

	if (pTherm)
	{
		success = true;
		pTherm->B = fix16_from_int(B);
		
		//Rinf = R25 * exp(-B/T0) = R25 / exp(B/T0)
		pTherm->Rinf = fix16_div(pTherm->B, T25CinKelvin);
		pTherm->Rinf = fix16_exp(pTherm->Rinf);
		// To allow for resistances over 32K, divide by 4
		pTherm->Rinf = fix16_div(fix16_from_int((R25+2)/4), pTherm->Rinf);
		pTherm->Rinf = fix16_mul(pTherm->Rinf, fix16_from_int(4));
	}
	
	return success;
}

fix16_t THERMISTOR_GetReading(THERMISTOR * pTherm, uint32_t R)
{
	// High resistances might overflow calculations - divide until within limits
	uint8_t divCount = 0;
	
	while (R > 32767)
	{
		R = (R + 1) / 2;
		divCount++;
	}
	fix16_t t = fix16_div(fix16_from_int(R), pTherm->Rinf);
	t = fix16_log(t);
	
	// Undo any divisions
	while (divCount--)
	{
		t = fix16_add(t, log2);
	}
	
	t = fix16_div(pTherm->B, t);
	return fix16_sub(t, T0CinKelvin);
}

uint32_t THERMISTOR_GetResistance(THERMISTOR * pTherm, fix16_t t)
{
	// Convert celcius to kelvin
	t = fix16_add(t, T0CinKelvin);
	
	fix16_t r = fix16_div(pTherm->B, t);
	r = fix16_div(r, fix16_from_int(2)); // Division by 2 to keep value in 16.16 range during exponentiation
	r = fix16_exp(r);
	
	uint32_t Rinf100 = (uint32_t)fix16_to_int(fix16_mul(pTherm->Rinf, fix16_from_int(100)));
	
	uint32_t res = (uint32_t)fix16_to_int(r);
	res = res * res; // Square to undo division by 2
	res *= Rinf100;
	res = (res + 50) / 100;
	return res;
}

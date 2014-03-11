/*
 * Standard library includes
 */

#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stddef.h>
#include <math.h>

/*
 * Utility Library Includes
 */

#include "util_fixedpoint.h"

/*
 * Device Library Includes
 */

#include "lib_pot_divider.h"
#include "lib_thermistor.h"

/*
 * Private defines and typedefs
 */

/*
 * Private Variables
 */

static FIXED_POINT_TYPE T25CinKelvin;
static FIXED_POINT_TYPE T0CinKelvin;
static FIXED_POINT_TYPE log2;

/*
 * Private Function Prototypes
 */

/*
 * Public Functions
 */

void THERMISTOR_Init(void)
{
	T25CinKelvin = fp_div(fp_from_int(29815), fp_from_int(100));
	T0CinKelvin = fp_div(fp_from_int(27315), fp_from_int(100));
	log2 = fp_log(fp_from_int(2));
}

bool THERMISTOR_InitDevice(THERMISTOR * pTherm, uint16_t B, uint32_t R25)
{
	bool success = false;

	if (pTherm)
	{
		success = true;
		pTherm->B = fp_from_int(B);
		
		//Rinf = R25 * exp(-B/T0) = R25 / exp(B/T0)
		pTherm->Rinf = fp_div(pTherm->B, T25CinKelvin);
		pTherm->Rinf = fp_exp(pTherm->Rinf);
		// To allow for resistances over 32K, divide by 4
		pTherm->Rinf = fp_div(fp_from_int((R25+2)/4), pTherm->Rinf);
		pTherm->Rinf = fp_mul(pTherm->Rinf, fp_from_int(4));
	}
	
	return success;
}

FIXED_POINT_TYPE THERMISTOR_TemperatureFromResistance(THERMISTOR * pTherm, uint32_t R)
{
	FIXED_POINT_TYPE t;

	#ifdef USE_FIX16
	
	uint8_t divCount = 0;
	
	// High resistance will overflow - divide until within limits
	t = fp_from_int(R);

	while (R > 32768)
	{
		R = R / 2;
		divCount++;
		t = fp_from_int(R);
	}
	
	// Low Rinf might overflow calculations - divide resistance until within limits
	t = fp_div(t, pTherm->Rinf);
	while (t == fix16_overflow)
	{
		R = R / 2;
		divCount++;
		t = fp_from_int(R);
		t = fp_div(t, pTherm->Rinf);
	}
	
	t = fp_log(t);

	// Undo any divisions
	while (divCount--)
	{
		t = fp_add(t, log2);
	}
	
	#else
	t = fp_from_int(R);
	t = fp_div(t, pTherm->Rinf);
	t = fp_log(t);
	#endif
	
	t = fp_div(pTherm->B, t);
	return fp_sub(t, T0CinKelvin);
}

FIXED_POINT_TYPE THERMISTOR_TemperatureFromADCReading(THERMISTOR * pTherm, POT_DIVIDER * pDivider, uint16_t reading)
{
	FIXED_POINT_TYPE result = fp_from_int(0);
	
	uint32_t rTherm = POTDIVIDER_GetResistanceFromADC(pDivider, reading);
	
	if (rTherm > 0)
	{
		result = THERMISTOR_TemperatureFromResistance(pTherm, rTherm);
	}
	
	return result;
}

uint32_t THERMISTOR_GetResistanceFromTemperature(THERMISTOR * pTherm, FIXED_POINT_TYPE t)
{
	// Convert celcius to kelvin
	t = fp_add(t, T0CinKelvin);
	
	FIXED_POINT_TYPE r = fp_div(pTherm->B, t);
	uint32_t res;
	
	#if USE_FIX16
	r = fp_div(r, fp_from_int(2)); // Division by 2 to keep value in 16.16 range during exponentiation
	r = fp_exp(r);
	
	// Use Rinf x100 to get enough significant figures
	uint32_t Rinf100 = (uint32_t)fp_to_int(fp_mul(pTherm->Rinf, fp_from_int(100)));
	
	res = (uint32_t)fp_to_int(r);
	res = res * res; // Square to undo division by 2
	res *= Rinf100;
	res = (res + 50) / 100; // Undo multiplication by 100
	
	#else
	res = fp_exp(r) * pTherm->Rinf;
	#endif
	
	return res;
}

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
 
#ifndef USE_FIX16
#define t_div(x, y) (x / y)
#define t_log(x) log(x)
#define t_from_int(x) (float)x
#define t_to_int(x) ((uint32_t)(x))
#define t_add(x, y) (x + y)
#define t_sub(x, y) (x - y)
#define t_mul(x, y) (x * y)
#define t_exp(x)	(exp(x))
#else
#define t_div(x, y) fix16_div(x, y)
#define t_log(x) fix16_log(x)
#define t_from_int(x) fix16_from_int(x)
#define t_to_int(x) fix16_to_int(x)
#define t_add(x, y) fix16_add(x, y)
#define t_sub(x, y) fix16_sub(x, y)
#define t_mul(x, y) fix16_mul(x, y)
#define t_exp(x)	fix16_exp(x)
#endif

/*
 * Private Variables
 */

static DECIMAL_TYPE T25CinKelvin;
static DECIMAL_TYPE T0CinKelvin;
static DECIMAL_TYPE log2;

/*
 * Private Function Prototypes
 */

/*
 * Public Functions
 */

void THERMISTOR_Init(void)
{
	T25CinKelvin = t_div(t_from_int(29815), t_from_int(100));
	T0CinKelvin = t_div(t_from_int(27315), t_from_int(100));
	log2 = t_log(t_from_int(2));
}

bool THERMISTOR_InitDevice(THERMISTOR * pTherm, uint16_t B, uint32_t R25)
{
	bool success = false;

	if (pTherm)
	{
		success = true;
		pTherm->B = t_from_int(B);
		
		//Rinf = R25 * exp(-B/T0) = R25 / exp(B/T0)
		pTherm->Rinf = t_div(pTherm->B, T25CinKelvin);
		pTherm->Rinf = t_exp(pTherm->Rinf);
		// To allow for resistances over 32K, divide by 4
		pTherm->Rinf = t_div(t_from_int((R25+2)/4), pTherm->Rinf);
		pTherm->Rinf = t_mul(pTherm->Rinf, t_from_int(4));
	}
	
	return success;
}

bool THERMISTOR_InitDivider(THERMISTOR_DIVIDER_READING * pDivider, uint16_t maxReading, uint32_t rPulldown)
{
	bool success = false;

	if (pDivider)
	{
		success = true;
		pDivider->maxReading = maxReading;
		pDivider->rPulldown = rPulldown;
	}
	
	return success;
}

DECIMAL_TYPE THERMISTOR_GetReading(THERMISTOR * pTherm, uint32_t R)
{
	uint8_t divCount = 0;
	DECIMAL_TYPE t;

	#ifdef USE_FIX16
	// High resistance will overflow - divide until within limits
	t = t_from_int(R);

	while (R > 32768)
	{
		R = R / 2;
		divCount++;
		t = t_from_int(R);
	}
	
	// Low Rinf might overflow calculations - divide resistance until within limits
	t = t_div(t, pTherm->Rinf);
	while (t == fix16_overflow)
	{
		R = R / 2;
		divCount++;
		t = t_from_int(R);
		t = t_div(t, pTherm->Rinf);
	}
	
	t = t_log(t);

	// Undo any divisions
	while (divCount--)
	{
		t = t_add(t, log2);
	}
	
	#else
	t = t_from_int(R);
	t = t_div(t, pTherm->Rinf);
	t = t_log(t);
	#endif
	
	t = t_div(pTherm->B, t);
	return t_sub(t, T0CinKelvin);
}

DECIMAL_TYPE THERMISTOR_GetDividerReading(THERMISTOR * pTherm, THERMISTOR_DIVIDER_READING * pDivider, uint16_t reading)
{
	if ((reading > 0) && (reading < pDivider->maxReading))
	{
		uint32_t r = (pDivider->rPulldown * pDivider->maxReading) / reading;
		r -= pDivider->rPulldown;
		return THERMISTOR_GetReading(pTherm, r);
	}
	else
	{
		return t_from_int(0);
	}
}

uint32_t THERMISTOR_GetResistance(THERMISTOR * pTherm, DECIMAL_TYPE t)
{
	// Convert celcius to kelvin
	t = t_add(t, T0CinKelvin);
	
	DECIMAL_TYPE r = t_div(pTherm->B, t);
	uint32_t res;
	
	#if USE_FIX16
	r = t_div(r, t_from_int(2)); // Division by 2 to keep value in 16.16 range during exponentiation
	r = t_exp(r);
	
	// Use Rinf x100 to get enough significant figures
	uint32_t Rinf100 = (uint32_t)t_to_int(t_mul(pTherm->Rinf, t_from_int(100)));
	
	res = (uint32_t)t_to_int(r);
	res = res * res; // Square to undo division by 2
	res *= Rinf100;
	res = (res + 50) / 100; // Undo multiplication by 100
	
	#else
	res = t_exp(r) * pTherm->Rinf;
	#endif
	
	return res;
}

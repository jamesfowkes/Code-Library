#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

#include "unity.h"

#include "util_fixedpoint.h"
#include "lib_thermistor.h"

#define RESISTANCE_AT_25C 10000UL
#define BETA 3000U

THERMISTOR t;
THERMISTOR_DIVIDER_READING divider;

char fpBuffer1[13];
char fpBuffer2[13];
char messageBuffer[256];

void setUp(void)
{

}

void tearDown(void)
{

}

void test_ThermistorInit(void)
{
	THERMISTOR_Init();
	TEST_ASSERT_EQUAL(false, THERMISTOR_InitDevice(NULL, 0, 0));
	TEST_ASSERT_EQUAL(true, THERMISTOR_InitDevice(&t, BETA, RESISTANCE_AT_25C));
	TEST_ASSERT_EQUAL(true, THERMISTOR_InitDivider(&divider, 1023, RESISTANCE_AT_25C));
	TEST_ASSERT_EQUAL(1023, divider.maxReading);
	TEST_ASSERT_EQUAL(RESISTANCE_AT_25C, divider.rPulldown);
}

void test_ValidAt25Degrees(void)
{
	FIXED_POINT_TYPE temp = THERMISTOR_GetReading(&t, RESISTANCE_AT_25C);
	TEST_ASSERT_EQUAL(25, fp_to_int(temp));

	uint32_t r = THERMISTOR_GetResistance(&t, fp_from_int(25));
	
	// Readings should be within 1% of expected
	TEST_ASSERT_UINT_WITHIN((RESISTANCE_AT_25C+50)/100, RESISTANCE_AT_25C, r);
	
	temp = THERMISTOR_GetReading(&t, r);
	TEST_ASSERT_EQUAL(25, fp_to_int(temp));
}

void test_ValidBetweenMinus30And50(void)
{
	uint32_t rHiTemp = THERMISTOR_GetResistance(&t, fp_from_int(50));
	uint32_t rLoTemp = THERMISTOR_GetResistance(&t, fp_from_int(-30));
	
	uint32_t r;
	FIXED_POINT_TYPE temp;
	// Test valid conversions between -30 and 50 degrees celcius

	for (r = rHiTemp; r < rLoTemp; ++r)
	{
		temp = THERMISTOR_GetReading(&t, r);
		fp_to_str(t.Rinf, fpBuffer1, 2);
		fp_to_str(temp, fpBuffer2, 2);
		sprintf(messageBuffer, "R = %lu, Rinf = %s, t=%s (MaxR=%lu, MinR=%lu)", r, fpBuffer1, fpBuffer2, rLoTemp, rHiTemp);
				
		TEST_ASSERT_MESSAGE(fp_to_int(temp) <= 51, messageBuffer);
		TEST_ASSERT_MESSAGE(fp_to_int(temp) >= -31, messageBuffer);
	}
}

void test_Divider(void)
{
	uint16_t adcReading;
	FIXED_POINT_TYPE temp;

	temp = THERMISTOR_GetDividerReading(&t, &divider, 512);
	TEST_ASSERT_UINT_WITHIN(1, 25, fp_to_int(temp));
	
	for (adcReading = 1; adcReading < 1023; adcReading++)
	{
		temp = THERMISTOR_GetDividerReading(&t, &divider, adcReading);
		TEST_ASSERT(temp > fp_from_int(-1000));
		TEST_ASSERT(temp < fp_from_int(1000));
	}
}

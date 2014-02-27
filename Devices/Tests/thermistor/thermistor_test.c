#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

#include "unity.h"

#include "fix16.h"
#include "lib_thermistor.h"

#define RESISTANCE_AT_25C 100000UL
#define BETA 3000U

THERMISTOR t;
char buf[13];

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
}

void test_Readings(void)
{
	fix16_t temp = THERMISTOR_GetReading(&t, RESISTANCE_AT_25C);
	TEST_ASSERT_EQUAL(25, fix16_to_int(temp));

	uint32_t r = THERMISTOR_GetResistance(&t, fix16_from_int(25));
	TEST_ASSERT_UINT_WITHIN((RESISTANCE_AT_25C+100)/200, RESISTANCE_AT_25C, r);
	
	temp = THERMISTOR_GetReading(&t, r);
	TEST_ASSERT_EQUAL(25, fix16_to_int(temp));
	
	uint32_t rHiTemp = THERMISTOR_GetResistance(&t, fix16_from_int(50));
	uint32_t rLoTemp = THERMISTOR_GetResistance(&t, fix16_from_int(-30));
	
	printf("Calculating temperatures from R=%lu to R=%lu\n", rHiTemp, rLoTemp);
	for (r = rHiTemp; r < rLoTemp; ++r)
	{
		temp = THERMISTOR_GetReading(&t, r);
		TEST_ASSERT(fix16_to_int(temp) <= 50);
		TEST_ASSERT(fix16_to_int(temp) >= -30);
	}
}


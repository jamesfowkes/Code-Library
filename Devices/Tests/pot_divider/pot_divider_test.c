#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

#include "unity.h"

#include "util_fixedpoint.h"
#include "lib_pot_divider.h"


POT_DIVIDER puDivider;
POT_DIVIDER pdDivider;

char messageBuffer[256];

void setUp(void)
{

}

void tearDown(void)
{

}

void test_Initialisation(void)
{
	TEST_ASSERT_FALSE( POTDIVIDER_Init(NULL, 1023, 10000, PULLDOWN) );
	TEST_ASSERT( POTDIVIDER_Init(&puDivider, 1023, 10000, PULLUP) );
	TEST_ASSERT( POTDIVIDER_Init(&pdDivider, 1023, 10000, PULLDOWN) );
}

void test_Pulldown(void)
{
	for (uint16_t reading = 10; reading < 1013; reading++)
	{
		float expectedR = ((1023.0f*10000.0f)/(float)reading) - 10000.0f;
		uint32_t res = POTDIVIDER_GetResistanceFromADC(&pdDivider, reading);
		
		TEST_ASSERT_UINT_WITHIN(1, (uint32_t)(expectedR + 0.5f), res);
		TEST_ASSERT_UINT_WITHIN(1, POTDIVIDER_GetADCFromResistance(&pdDivider, res), reading);
	}
}

void test_Pullup(void)
{
	for (uint16_t reading = 10; reading < 1013; reading++)
	{
		float expectedR = ((float)reading * 10000.0f)/(1023.0f-(float)reading);
		uint32_t res = POTDIVIDER_GetResistanceFromADC(&puDivider, reading);

		TEST_ASSERT_UINT_WITHIN(1, (uint32_t)(expectedR + 0.5f), res);
		TEST_ASSERT_UINT_WITHIN(1, POTDIVIDER_GetADCFromResistance(&puDivider, res), reading);
	}
}

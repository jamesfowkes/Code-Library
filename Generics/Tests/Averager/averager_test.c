#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

#include "unity.h"

#include "../../averager.h"
#include "util_macros.h"

static AVERAGER *pAverager;

void setUp(void)
{
	pAverager = AVERAGER_GetAverager(20);
}

void tearDown(void)
{

}

void test_AveragerInit(void)
{
	TEST_ASSERT_NOT_EQUAL(NULL, pAverager);
}

void test_AveragerRunning(void)
{
	for (uint8_t i = 0; i < 100; ++i)
	{
		AVERAGER_NewData(pAverager, 1);
		TEST_ASSERT_EQUAL(1, AVERAGER_GetAverage(pAverager) );
	}
	
	// The average of these data is 18173 (rounded)
	uint32_t newData[] =
		{20555, 26169, 3383, 31579, 12439, 10241, 27172, 81, 16275, 29671, 20419, 9156, 18084, 13494, 17657, 28744, 27936, 30908, 2160, 17334};
	
	for (uint8_t i = 0; i < 20; ++i)
	{
		AVERAGER_NewData(pAverager, newData[i]);
	}
	
	
	TEST_ASSERT_EQUAL(18173, AVERAGER_GetAverage(pAverager));
	
}

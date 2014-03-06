#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

#include "unity.h"

#include "../../averager.h"
#include "util_macros.h"

struct averager8
{
	int8_t * data;
	uint8_t maxIndex;
	uint8_t iWrite;
	bool full;
};

struct averager16
{
	int16_t * data;
	uint8_t maxIndex;
	uint8_t iWrite;
	bool full;
};

struct averager32
{
	int32_t * data;
	uint8_t maxIndex;
	uint8_t iWrite;
	bool full;
};

static AVERAGER8 *pAverager8;
static AVERAGER16 *pAverager16;
static AVERAGER32 *pAverager32;

void setUp(void)
{

}

void tearDown(void)
{

}

void test_AveragerInit(void)
{

	pAverager8 = AVERAGER8_GetAverager(20);
	pAverager16 = AVERAGER16_GetAverager(20);
	pAverager32 = AVERAGER32_GetAverager(20);
	
	TEST_ASSERT_NOT_EQUAL(NULL, pAverager8);
	TEST_ASSERT_NOT_EQUAL(NULL, pAverager16);
	TEST_ASSERT_NOT_EQUAL(NULL, pAverager32);
}

void test_Averager8Running(void)
{
	for (uint8_t i = 0; i < 100; ++i)
	{
		AVERAGER8_NewData(pAverager8, 1);
		TEST_ASSERT_EQUAL(1, AVERAGER8_GetAverage(pAverager8) );
	}
	
	// The average of these data is 8 (rounded)
	int8_t newData[] =
		{58, -41, 103, 127, 104, -84, 80, -8, 4, -127, 50, -97, -69, 44, -57, 29, 25, 38, -101, 74};
	
	for (uint8_t i = 0; i < 20; ++i)
	{
		AVERAGER8_NewData(pAverager8, newData[i]);
	}
	
	TEST_ASSERT_EQUAL(8, AVERAGER8_GetAverage(pAverager8));
	
}

void test_Averager16Running(void)
{
	for (uint8_t i = 0; i < 100; ++i)
	{
		AVERAGER16_NewData(pAverager16, 1);
		TEST_ASSERT_EQUAL(1, AVERAGER16_GetAverage(pAverager16) );
	}
	
	// The average of these data is 5650 (rounded)
	int16_t newData[] =
		{-3739, 22893, 7805, 17336, -11750, 28826, -2848, 32719, 3500, 14826, -14924, -2126, 13481, -1671, -2755, 26115, -10519, -32498, 22731, 5599};
	
	for (uint8_t i = 0; i < 20; ++i)
	{
		AVERAGER16_NewData(pAverager16, newData[i]);
	}
	
	TEST_ASSERT_EQUAL(5650, AVERAGER16_GetAverage(pAverager16));
	
}

void test_Averager32Running(void)
{
	for (uint8_t i = 0; i < 100; ++i)
	{
		AVERAGER32_NewData(pAverager32, 1);
		TEST_ASSERT_EQUAL(1, AVERAGER32_GetAverage(pAverager32) );
	}
	
	// The average of these data is 18173 (rounded)
	uint32_t newData[] =
		{20555, 26169, 3383, 31579, 12439, 10241, 27172, 81, 16275, 29671, 20419, 9156, 18084, 13494, 17657, 28744, 27936, 30908, 2160, 17334};
	
	for (uint8_t i = 0; i < 20; ++i)
	{
		AVERAGER32_NewData(pAverager32, newData[i]);
	}
	
	
	TEST_ASSERT_EQUAL(18173, AVERAGER32_GetAverage(pAverager32));
	
}

void test_Averager8Reset(void)
{
	AVERAGER8_Reset(pAverager8, 0);
	TEST_ASSERT_EQUAL(0, AVERAGER8_GetAverage(pAverager8));
	AVERAGER8_Reset(pAverager8, 10);
	TEST_ASSERT_EQUAL(10, AVERAGER8_GetAverage(pAverager8));
}

void test_Averager16Reset(void)
{
	AVERAGER16_Reset(pAverager16, 0);
	TEST_ASSERT_EQUAL(0, AVERAGER16_GetAverage(pAverager16));
	AVERAGER16_Reset(pAverager16, 1000);
	TEST_ASSERT_EQUAL(1000, AVERAGER16_GetAverage(pAverager16));
}

void test_Averager32Reset(void)
{
	AVERAGER32_Reset(pAverager32, 0);
	TEST_ASSERT_EQUAL(0, AVERAGER32_GetAverage(pAverager32));
	AVERAGER32_Reset(pAverager32, 100000);
	TEST_ASSERT_EQUAL(100000, AVERAGER32_GetAverage(pAverager32));
}

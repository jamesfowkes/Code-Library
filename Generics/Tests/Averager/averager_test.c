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

struct averageru8
{
	uint8_t * data;
	uint8_t maxIndex;
	uint8_t iWrite;
	bool full;
};

struct averageru16
{
	uint16_t * data;
	uint8_t maxIndex;
	uint8_t iWrite;
	bool full;
};

struct averageru32
{
	uint32_t * data;
	uint8_t maxIndex;
	uint8_t iWrite;
	bool full;
};

static AVERAGER8 *pAverager8;
static AVERAGER16 *pAverager16;
static AVERAGER32 *pAverager32;

static AVERAGERU8 *pAverageru8;
static AVERAGERU16 *pAverageru16;
static AVERAGERU32 *pAverageru32;

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
	
	pAverageru8 = AVERAGERU8_GetAverager(20);
	pAverageru16 = AVERAGERU16_GetAverager(20);
	pAverageru32 = AVERAGERU32_GetAverager(20);
	
	TEST_ASSERT_NOT_EQUAL(NULL, pAverager8);
	TEST_ASSERT_NOT_EQUAL(NULL, pAverager16);
	TEST_ASSERT_NOT_EQUAL(NULL, pAverager32);	
	TEST_ASSERT_NOT_EQUAL(NULL, pAverageru8);
	TEST_ASSERT_NOT_EQUAL(NULL, pAverageru16);
	TEST_ASSERT_NOT_EQUAL(NULL, pAverageru32);
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

void test_Averageru8Running(void)
{
	for (uint8_t i = 0; i < 100; ++i)
	{
		AVERAGERU8_NewData(pAverageru8, 1);
		TEST_ASSERT_EQUAL(1, AVERAGERU8_GetAverage(pAverageru8) );
	}
	
	// The average of these data is 137 (rounded)
	uint8_t newData[] =
		{71,45,228,13,126,199,250,104,147,178,143,180,88,194,123,115,156,59,70,248};
	
	for (uint8_t i = 0; i < 20; ++i)
	{
		AVERAGERU8_NewData(pAverageru8, newData[i]);
	}
	
	TEST_ASSERT_EQUAL(137, AVERAGERU8_GetAverage(pAverageru8));
	
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

void test_Averageru16Running(void)
{
	for (uint8_t i = 0; i < 100; ++i)
	{
		AVERAGERU16_NewData(pAverageru16, 1);
		TEST_ASSERT_EQUAL(1, AVERAGERU16_GetAverage(pAverageru16) );
	}
	
	// The average of these data is 30019 (rounded)
	uint16_t newData[] =
		{59044,44841,799,40117,15411,3329,65183,44259,4952,53251,23130,41137,21522,31266,4668,45402,28737,4986,18550,49794};
	
	for (uint8_t i = 0; i < 20; ++i)
	{
		AVERAGERU16_NewData(pAverageru16, newData[i]);
	}
	
	TEST_ASSERT_EQUAL(30019, AVERAGERU16_GetAverage(pAverageru16));
}

void test_Averager32Running(void)
{
	for (uint8_t i = 0; i < 100; ++i)
	{
		AVERAGER32_NewData(pAverager32, 1);
		TEST_ASSERT_EQUAL(1, AVERAGER32_GetAverage(pAverager32) );
	}
	
	// The average of these data is 18173 (rounded)
	int32_t newData[] =
		{20555, 26169, 3383, 31579, 12439, 10241, 27172, 81, 16275, 29671, 20419, 9156, 18084, 13494, 17657, 28744, 27936, 30908, 2160, 17334};
	
	for (uint8_t i = 0; i < 20; ++i)
	{
		AVERAGER32_NewData(pAverager32, newData[i]);
	}
	
	
	TEST_ASSERT_EQUAL(18173, AVERAGER32_GetAverage(pAverager32));
}

void test_Averageru32Running(void)
{
	for (uint8_t i = 0; i < 100; ++i)
	{
		AVERAGERU32_NewData(pAverageru32, 1);
		TEST_ASSERT_EQUAL(1, AVERAGERU32_GetAverage(pAverageru32) );
	}
	
	// The average of these data is 137303231 (rounded)
	uint32_t newData[] =
		{247257524, 93168365, 213206737, 173374918, 130153833, 247899438, 230140165, 68239320, 
		109850936, 159456795, 144173778, 261536527, 3055151, 74545759, 3605332, 212496497, 
		31211219, 277576, 192502949, 149911801};
	
	for (uint8_t i = 0; i < 20; ++i)
	{
		AVERAGERU32_NewData(pAverageru32, newData[i]);
	}
	
	
	TEST_ASSERT_EQUAL(137303231, AVERAGERU32_GetAverage(pAverageru32));
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

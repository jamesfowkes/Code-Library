#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

#include "unity.h"

#include "../../integertypeenum.h"
#include "../../averager.h"
#include "../../memorypool.h"

#include "util_macros.h"

/*
 * Defines and Typedefs
 */

#define TEST_RESET(averager_type, data_type, expected_result_var) \
{ \
data_type result; \
pAverager = AVERAGER_GetAverager(averager_type, 20); \
\
AVERAGER_Reset(pAverager, &expected_result_var); \
AVERAGER_GetAverage(pAverager, &result); \
TEST_ASSERT_EQUAL(expected_result_var, result); \
\
AVERAGER_Reset(pAverager, NULL); \
AVERAGER_GetAverage(pAverager, &result); \
TEST_ASSERT_EQUAL(0, result); \
\
AVERAGER_Reset(pAverager, NULL); \
AVERAGER_GetAverage(pAverager, &result); \
TEST_ASSERT_EQUAL(0, result); \
}

#define TEST_RUNNING(averager_type, data_type, data_ptr, expected_result_var) \
{ \
data_type result; \
pAverager = AVERAGER_GetAverager(averager_type, 20); \
pCurrentData = data_ptr; \
fillRunningAverager(sizeof(data_type)); \
AVERAGER_GetAverage(pAverager, &result); \
TEST_ASSERT_EQUAL(expected_result_var, result); \
}

/*
 * Private Test Variables
 */
 
// The average of these data is 8 (rounded)
static int8_t s8data[] = {58, -41, 103, 127, 104, -84, 80, -8, 4, -127, 50, -97, -69, 44, -57, 29, 25, 38, -101, 74};
static int8_t s8dataAverage = 8;

// The average of these data is 137 (rounded)
static uint8_t u8data[] = {71,45,228,13,126,199,250,104,147,178,143,180,88,194,123,115,156,59,70,248};
static uint8_t u8dataAverage = 137;

// The average of these data is 5650 (rounded)
static int16_t s16data[] =
		{-3739, 22893, 7805, 17336, -11750, 28826, -2848, 32719, 3500, 14826, -14924, -2126, 13481, -1671, -2755, 26115, -10519, -32498, 22731, 5599};
static int16_t s16dataAverage = 5650;

// The average of these data is 30019 (rounded)
static uint16_t u16data[] =
		{59044,44841,799,40117,15411,3329,65183,44259,4952,53251,23130,41137,21522,31266,4668,45402,28737,4986,18550,49794};
static uint16_t u16dataAverage = 30019;

// The average of these data is 18173 (rounded)
static int32_t s32data[] =
		{20555, 26169, 3383, 31579, 12439, 10241, 27172, 81, 16275, 29671, 20419, 9156, 18084, 13494, 17657, 28744, 27936, 30908, 2160, 17334};
static int32_t s32dataAverage = 18173;

// The average of these data is 137303231 (rounded)
static uint32_t u32data[] =
		{247257524, 93168365, 213206737, 173374918, 130153833, 247899438, 230140165, 68239320, 
		109850936, 159456795, 144173778, 261536527, 3055151, 74545759, 3605332, 212496497, 
		31211219, 277576, 192502949, 149911801};	
static uint32_t u32dataAverage = 137303231;

static AVERAGER *pAverager;
static void *pCurrentData;

static int8_t s8resetValue = -10;
static uint8_t u8resetValue = 10;
static int16_t s16resetValue = -10;
static uint16_t u16resetValue = 10;
static int32_t s32resetValue = -10;
static uint32_t u32resetValue = 10;

void setUp(void)
{
	MEMPOOL_Reset();
}

void tearDown(void)
{

}

void test_AveragerInit(void)
{
	pAverager = AVERAGER_GetAverager(S8, 20);
	TEST_ASSERT_NOT_EQUAL(NULL, pAverager);
	pAverager = AVERAGER_GetAverager(U8, 20);
	TEST_ASSERT_NOT_EQUAL(NULL, pAverager);
	pAverager = AVERAGER_GetAverager(S16, 20);
	TEST_ASSERT_NOT_EQUAL(NULL, pAverager);
	pAverager = AVERAGER_GetAverager(U16, 20);
	TEST_ASSERT_NOT_EQUAL(NULL, pAverager);
	pAverager = AVERAGER_GetAverager(S32, 20);
	TEST_ASSERT_NOT_EQUAL(NULL, pAverager);
	pAverager = AVERAGER_GetAverager(U32, 20);
	TEST_ASSERT_NOT_EQUAL(NULL, pAverager);
}

static void fillRunningAverager(uint8_t size)
{
	for (uint8_t i = 0; i < 20; ++i)
	{
		AVERAGER_NewData(pAverager, pCurrentData + (i*size));
	}	
}

void test_AveragerS8Running(void) {	TEST_RUNNING(S8, int8_t, s8data, s8dataAverage); }
void test_AveragerU8Running(void) {	TEST_RUNNING(U8, uint8_t, u8data, u8dataAverage); }
void test_AveragerS16Running(void) { TEST_RUNNING(S16, int16_t, s16data, s16dataAverage); }
void test_AveragerU16Running(void) { TEST_RUNNING(U16, uint16_t, u16data, u16dataAverage); }
void test_AveragerS32Running(void) { TEST_RUNNING(S32, int32_t, s32data, s32dataAverage); }
void test_AveragerU32Running(void) { TEST_RUNNING(U32, uint32_t, u32data, u32dataAverage); }

void test_AveragerS8Reset(void) { TEST_RESET(S8, int8_t, s8resetValue); }
void test_AveragerU8Reset(void) { TEST_RESET(U8, uint8_t, u8resetValue); }
void test_AveragerS16Reset(void) { TEST_RESET(S16, int16_t, s16resetValue); }
void test_AveragerU16Reset(void) { TEST_RESET(U16, uint16_t, u16resetValue); }
void test_AveragerS32Reset(void) { TEST_RESET(S16, int16_t, s32resetValue); }
void test_AveragerU32Reset(void) { TEST_RESET(U16, uint16_t, u32resetValue); }

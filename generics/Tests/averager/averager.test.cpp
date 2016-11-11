#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/extensions/HelperMacros.h>

#include "../../integertypeenum.h"
#include "../../averager.h"
#include "../../memorypool.h"

#include "Utility/util_macros.h"

/*
 * Defines and Typedefs
 */

/*
 * Private Test Variables
 */
 
// The average of these data is 8 (rounded)
static int8_t s8data[] = {58, -41, 103, 127, 104, -84, 80, -8, 4, -127, 50, -97, -69, 44, -57, 29, 25, 38, -101, 74};
static int8_t s8dataAverage = 8;

// The average of these data is 137 (rounded)
static uint8_t u8data[] = {71, 45, 228, 13, 126, 199, 250, 104, 147, 178, 143, 180, 88, 194, 123, 115, 156, 59, 70, 248};
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

template <typename T>
static void fillRunningAverager()
{
	for (uint8_t i = 0; i < 20; ++i)
	{
		AVERAGER_NewData(pAverager, &((T*)pCurrentData)[i]);
	}	
}

template <typename T>
static void test_reset(INTEGERTYPE averager_type, T expected_result_var)
{
	T result;
	pAverager = AVERAGER_GetAverager(averager_type, 20);

	AVERAGER_Reset(pAverager, &expected_result_var);
	AVERAGER_GetAverage(pAverager, &result);
	CPPUNIT_ASSERT_EQUAL((T)expected_result_var, result);

	AVERAGER_Reset(pAverager, NULL);
	AVERAGER_GetAverage(pAverager, &result);
	CPPUNIT_ASSERT_EQUAL((T)0, result);

	AVERAGER_Reset(pAverager, NULL);
	AVERAGER_GetAverage(pAverager, &result);
	CPPUNIT_ASSERT_EQUAL((T)0, result);
}

template <typename T>
static void test_running(INTEGERTYPE averager_type, void * data_ptr, T expected_result_var)
{
	T result;
	pAverager = AVERAGER_GetAverager(averager_type, 20);
	pCurrentData = data_ptr;
	fillRunningAverager<T>();
	AVERAGER_GetAverage(pAverager, &result);
	CPPUNIT_ASSERT_EQUAL(expected_result_var, result);
}

class AveragerTest : public CppUnit::TestFixture  {

	CPPUNIT_TEST_SUITE(AveragerTest);
	CPPUNIT_TEST(test_AveragerInit);
	CPPUNIT_TEST(test_AveragerS8Running);
	CPPUNIT_TEST(test_AveragerU8Running);
	CPPUNIT_TEST(test_AveragerS16Running);
	CPPUNIT_TEST(test_AveragerU16Running);
	CPPUNIT_TEST(test_AveragerS32Running);
	CPPUNIT_TEST(test_AveragerU32Running);
	CPPUNIT_TEST(test_AveragerS8Reset);
	CPPUNIT_TEST(test_AveragerU8Reset);
	CPPUNIT_TEST(test_AveragerS16Reset);
	CPPUNIT_TEST(test_AveragerU16Reset);
	CPPUNIT_TEST(test_AveragerS32Reset);
	CPPUNIT_TEST(test_AveragerU32Reset);
	CPPUNIT_TEST_SUITE_END();

	void test_AveragerInit(void)
	{
		pAverager = AVERAGER_GetAverager(S8, 20);
		CPPUNIT_ASSERT(NULL != pAverager);
		pAverager = AVERAGER_GetAverager(U8, 20);
		CPPUNIT_ASSERT(NULL != pAverager);
		pAverager = AVERAGER_GetAverager(S16, 20);
		CPPUNIT_ASSERT(NULL != pAverager);
		pAverager = AVERAGER_GetAverager(U16, 20);
		CPPUNIT_ASSERT(NULL != pAverager);
		pAverager = AVERAGER_GetAverager(S32, 20);
		CPPUNIT_ASSERT(NULL != pAverager);
		pAverager = AVERAGER_GetAverager(U32, 20);
		CPPUNIT_ASSERT(NULL != pAverager);
	}

	void test_AveragerS8Running(void) {	test_running<int8_t>(S8, s8data, s8dataAverage); }
	void test_AveragerU8Running(void) {	test_running<uint8_t>(U8, u8data, u8dataAverage); }
	void test_AveragerS16Running(void) { test_running<int16_t>(S16, s16data, s16dataAverage); }
	void test_AveragerU16Running(void) { test_running<uint16_t>(U16, u16data, u16dataAverage); }
	void test_AveragerS32Running(void) { test_running<int32_t>(S32, s32data, s32dataAverage); }
	void test_AveragerU32Running(void) { test_running<uint32_t>(U32, u32data, u32dataAverage); }

	void test_AveragerS8Reset(void) { test_reset<int8_t>(S8, s8resetValue); }
	void test_AveragerU8Reset(void) { test_reset<uint8_t>(U8, u8resetValue); }
	void test_AveragerS16Reset(void) { test_reset<int16_t>(S16, s16resetValue); }
	void test_AveragerU16Reset(void) { test_reset<uint16_t>(U16, u16resetValue); }
	void test_AveragerS32Reset(void) { test_reset<int16_t>(S32, s32resetValue); }
	void test_AveragerU32Reset(void) { test_reset<uint16_t>(U32, u32resetValue); }

	public:

	void setUp(void)
	{
		MEMPOOL_Reset();
	}

	void tearDown(void)
	{

	}
};

int main()
{
   CppUnit::TextUi::TestRunner runner;
   
   CPPUNIT_TEST_SUITE_REGISTRATION( AveragerTest );

   CppUnit::TestFactoryRegistry &registry = CppUnit::TestFactoryRegistry::getRegistry();

   runner.addTest( registry.makeTest() );
   runner.run();

   return 0;
}
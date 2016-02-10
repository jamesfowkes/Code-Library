#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

#include <limits>

#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/extensions/HelperMacros.h>

#include "Utility/util_fixedpoint.h"

#include "lib_pot_divider.h"

POT_DIVIDER puDivider;
POT_DIVIDER pdDivider;

char messageBuffer[256];

class PotDividerTest : public CppUnit::TestFixture  {

	CPPUNIT_TEST_SUITE(PotDividerTest);
	CPPUNIT_TEST(test_Initialisation);
	CPPUNIT_TEST(test_FixedPulldown);
	CPPUNIT_TEST(test_FixedPullup);
	CPPUNIT_TEST_SUITE_END();

	void test_Initialisation(void)
	{
		CPPUNIT_ASSERT( !POTDIVIDER_Init(NULL, 1023, 10000, FIXED_PULLDOWN) );
		CPPUNIT_ASSERT( POTDIVIDER_Init(&puDivider, 1023, 10000, FIXED_PULLUP) );
		CPPUNIT_ASSERT( POTDIVIDER_Init(&pdDivider, 1023, 10000, FIXED_PULLDOWN) );
	}

	void test_FixedPulldown(void)
	{

		float expectedR = POTDIVIDER_GetInfiniteResitance();
		uint32_t res = POTDIVIDER_GetResistanceFromADC(&pdDivider, 0);

		CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedR, res, 1);
		CPPUNIT_ASSERT_EQUAL((uint16_t)1023,  POTDIVIDER_GetADCFromResistance(&pdDivider, res));
			
		for (uint16_t reading = 1; reading < 1023; reading++)
		{
			expectedR = ((1023.0f/(float)reading) - 1) * 10000.0f;
			res = POTDIVIDER_GetResistanceFromADC(&pdDivider, reading);
			
			CPPUNIT_ASSERT_DOUBLES_EQUAL(expectedR , res, 1);
			CPPUNIT_ASSERT_EQUAL((uint16_t)reading, POTDIVIDER_GetADCFromResistance(&pdDivider, res));
		}
	}

	void test_FixedPullup(void)
	{
		for (uint16_t reading = 0; reading < 1023; reading++)
		{
			float expectedR = ((float)reading * 10000.0f)/(1023.0f-(float)reading);
			uint32_t res = POTDIVIDER_GetResistanceFromADC(&puDivider, reading);

			CPPUNIT_ASSERT_DOUBLES_EQUAL((expectedR + 0.5f), res, 1);
			CPPUNIT_ASSERT_EQUAL((uint16_t)reading, POTDIVIDER_GetADCFromResistance(&puDivider, res));
		}
	}
};

int main()
{
   CppUnit::TextUi::TestRunner runner;
   
   CPPUNIT_TEST_SUITE_REGISTRATION( PotDividerTest );

   CppUnit::TestFactoryRegistry &registry = CppUnit::TestFactoryRegistry::getRegistry();

   runner.addTest( registry.makeTest() );
   runner.run();

   return 0;
}

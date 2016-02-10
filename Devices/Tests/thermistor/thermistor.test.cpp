#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>


#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/extensions/HelperMacros.h>

#include "Utility/util_fixedpoint.h"
#include "lib_pot_divider.h"
#include "lib_thermistor.h"

#define RESISTANCE_AT_25C 10000UL
#define BETA 3000U

THERMISTOR t;
POT_DIVIDER puDivider;
POT_DIVIDER pdDivider;

uint32_t rHiTemp;
uint32_t rLoTemp;

uint16_t adcHiTempPullup;
uint16_t adcLoTempPullup;
uint16_t adcHiTempPulldown;
uint16_t adcLoTempPulldown;

char fpBuffer1[13];
char fpBuffer2[13];
char messageBuffer[256];

class ThermistorTest : public CppUnit::TestFixture  {

	CPPUNIT_TEST_SUITE(ThermistorTest);
	CPPUNIT_TEST(test_initDevice);
	CPPUNIT_TEST(test_ValidAt25Degrees);
	CPPUNIT_TEST(test_ValidBetweenMinus30And50);
	CPPUNIT_TEST(test_PullupDivider);
	CPPUNIT_TEST(test_PulldownDivider);
	CPPUNIT_TEST_SUITE_END();

	void test_initDevice(void)
	{
		THERMISTOR_Init();
		CPPUNIT_ASSERT_EQUAL(false, THERMISTOR_InitDevice(NULL, 0, 0));
		CPPUNIT_ASSERT_EQUAL(true, THERMISTOR_InitDevice(&t, BETA, RESISTANCE_AT_25C));
	}

	void test_ValidAt25Degrees(void)
	{
		FIXED_POINT_TYPE temp = THERMISTOR_TemperatureFromResistance(&t, RESISTANCE_AT_25C);
		CPPUNIT_ASSERT_EQUAL(25, fp_to_int(temp));

		uint32_t r = THERMISTOR_GetResistanceFromTemperature(&t, fp_from_int(25));
		
		// Readings should be within 1% of expected
		CPPUNIT_ASSERT_DOUBLES_EQUAL(RESISTANCE_AT_25C, r, (RESISTANCE_AT_25C+50)/100);
		
		temp = THERMISTOR_TemperatureFromResistance(&t, r);
		CPPUNIT_ASSERT_EQUAL(25, fp_to_int(temp));
	}

	void test_ValidBetweenMinus30And50(void)
	{
		uint32_t r;
		FIXED_POINT_TYPE temp;
		// Test valid conversions between -30 and 50 degrees celcius

		for (r = rHiTemp; r < rLoTemp; ++r)
		{
			temp = THERMISTOR_TemperatureFromResistance(&t, r);
			fp_to_str(t.Rinf, fpBuffer1, 2);
			fp_to_str(temp, fpBuffer2, 2);
			sprintf(messageBuffer, "R = %lu, Rinf = %s, t=%s (MaxR=%lu, MinR=%lu)", r, fpBuffer1, fpBuffer2, rLoTemp, rHiTemp);
					
			CPPUNIT_ASSERT_MESSAGE(messageBuffer, fp_to_int(temp) <= 51);
			CPPUNIT_ASSERT_MESSAGE(messageBuffer, fp_to_int(temp) >= -31);
		}
	}

	void test_PullupDivider(void)
	{
		uint16_t adcReading;
		FIXED_POINT_TYPE temp;

		temp = THERMISTOR_TemperatureFromADCReading(&t, &puDivider, 512);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(25, fp_to_int(temp), 1);
		
		float expectedTemp;
		float resistance;
		
		for (adcReading = adcLoTempPullup; adcReading < adcHiTempPullup; adcReading++)
		{
			resistance = ((float)adcReading * (float)RESISTANCE_AT_25C)/(1023.0f-(float)adcReading);
			expectedTemp = 1.0f/(1.0f/298.15f+(1.0f/(float)BETA)*log(resistance/(float)RESISTANCE_AT_25C));
			
			temp = THERMISTOR_TemperatureFromADCReading(&t, &puDivider, adcReading);
			
			sprintf(messageBuffer, "Expected %f, got %f with reading %d", expectedTemp, temp, adcReading);
			CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE(messageBuffer, expectedTemp/100.0f, expectedTemp, temp);
		}
	}

	void test_PulldownDivider(void)
	{
		uint16_t adcReading;
		FIXED_POINT_TYPE temp;

		temp = THERMISTOR_TemperatureFromADCReading(&t, &pdDivider, 512);
		CPPUNIT_ASSERT_DOUBLES_EQUAL(25, fp_to_int(temp), 1);
		
		float expectedTemp;
		float resistance;
		
		for (adcReading = adcHiTempPulldown; adcReading < adcLoTempPulldown; adcReading++)
		{
			resistance = ((1023.0f * (float)RESISTANCE_AT_25C)/(float)adcReading) - (float)RESISTANCE_AT_25C;
			expectedTemp = 1.0f/(1.0f/298.15f+(1.0f/(float)BETA)*log(resistance/(float)RESISTANCE_AT_25C));
			
			temp = THERMISTOR_TemperatureFromADCReading(&t, &pdDivider, adcReading);
			
			sprintf(messageBuffer, "Expected %f, got %f with reading %d", expectedTemp, temp, adcReading);
			CPPUNIT_ASSERT_DOUBLES_EQUAL_MESSAGE(messageBuffer, expectedTemp/100.0f, expectedTemp, temp);
		}
	}

	public:
	void setUp(void)
	{
		POTDIVIDER_Init(&pdDivider, 1023, RESISTANCE_AT_25C, FIXED_PULLDOWN);
		POTDIVIDER_Init(&puDivider, 1023, RESISTANCE_AT_25C, FIXED_PULLUP);
		
		rHiTemp = THERMISTOR_GetResistanceFromTemperature(&t, fp_from_int(50));
		rLoTemp = THERMISTOR_GetResistanceFromTemperature(&t, fp_from_int(-30));
		
		adcHiTempPullup = POTDIVIDER_GetADCFromResistance(&puDivider, rHiTemp);
		adcLoTempPullup = POTDIVIDER_GetADCFromResistance(&puDivider, rLoTemp);
		adcHiTempPulldown = POTDIVIDER_GetADCFromResistance(&pdDivider, rHiTemp);
		adcLoTempPulldown = POTDIVIDER_GetADCFromResistance(&pdDivider, rLoTemp);
	}

	void tearDown(void)
	{

	}
};

int main()
{
   CppUnit::TextUi::TestRunner runner;
   
   CPPUNIT_TEST_SUITE_REGISTRATION( ThermistorTest );

   CppUnit::TestFactoryRegistry &registry = CppUnit::TestFactoryRegistry::getRegistry();

   runner.addTest( registry.makeTest() );
   runner.run();

   return 0;
}
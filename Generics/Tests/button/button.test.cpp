#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/extensions/HelperMacros.h>

#include "button.h"

static void repeatCallback(void);
static void changeStateCallback(BTN_STATE_ENUM state);

static BTN s_button_under_test = {
	BTN_STATE_INACTIVE,
	changeStateCallback,
	repeatCallback,
	0,
	100,
	0,
	100,
};

static BTN bad_button_1 = {
	BTN_STATE_INACTIVE,
	NULL,
	NULL,
	0,
	0,
	0,
	0,
};

static int repeatCount = 0;
static int changeStateCount = 0;

static void repeatCallback(void)
{
	repeatCount++;
}

static void changeStateCallback(BTN_STATE_ENUM state)
{
	(void)state;
	repeatCount = 0;
	changeStateCount++;
}

class ButtonTest : public CppUnit::TestFixture  {

	CPPUNIT_TEST_SUITE(ButtonTest);
	CPPUNIT_TEST(test_InitHandler);
	CPPUNIT_TEST(test_Update);
	CPPUNIT_TEST(test_UpdateWithDebouncing);
	CPPUNIT_TEST(test_NoRepeatInit);
	CPPUNIT_TEST(test_NoRepeatOnlyInvokesCallbacksOnce);
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp(void)
	{
		BTN good_button = {
			BTN_STATE_INACTIVE,
			changeStateCallback,
			repeatCallback,
			0,
			100,
			0,
			100
		};
		memcpy(&s_button_under_test, &good_button, sizeof(BTN));
		
		repeatCount = 0;
		changeStateCount = 0;
	}

	void tearDown(void)
	{

	}

protected:
	void test_InitHandler(void)
	{
		CPPUNIT_ASSERT_EQUAL(true, BTN_InitHandler(&s_button_under_test));
		
		// Get a non-const pointer to the button integers so we can tweak the constant values for testing
		uint16_t * pmax_repeat_count = (uint16_t *)(&s_button_under_test.max_repeat_count);
		uint16_t * pmax_debounce_count = (uint16_t *)(&s_button_under_test.max_debounce_count);
		
		*pmax_repeat_count = 0;
		*pmax_debounce_count = 100;
		CPPUNIT_ASSERT_EQUAL(false, BTN_InitHandler(&s_button_under_test));
		
		*pmax_repeat_count = 100;
		*pmax_debounce_count = 0;
		CPPUNIT_ASSERT_EQUAL(false, BTN_InitHandler(&s_button_under_test));
	}

	void test_Update(void)
	{
		BTN_Update(&s_button_under_test, BTN_STATE_INACTIVE);
		
		CPPUNIT_ASSERT_EQUAL(0, repeatCount);
		CPPUNIT_ASSERT_EQUAL(0, changeStateCount);
		
		int i;
		
		/* No button press */
		for (i = 0; i < 200; i++)
		{
			BTN_Update(&s_button_under_test, BTN_STATE_INACTIVE);
		}
		
		CPPUNIT_ASSERT_EQUAL(0, repeatCount);
		CPPUNIT_ASSERT_EQUAL(0, changeStateCount);
		
		/* Active button press up to 99*/
		for (i = 0; i < 99; i++)
		{
			BTN_Update(&s_button_under_test, BTN_STATE_ACTIVE);
		}
		
		CPPUNIT_ASSERT_EQUAL(0, repeatCount);
		CPPUNIT_ASSERT_EQUAL(0, changeStateCount);
		
		/* One more press changes state*/
		BTN_Update(&s_button_under_test, BTN_STATE_ACTIVE);
		
		CPPUNIT_ASSERT_EQUAL(0, repeatCount);
		CPPUNIT_ASSERT_EQUAL(1, changeStateCount);
		
		/* Continue in active state for 99 counts */
		for (i = 0; i < 99; i++)
		{
			BTN_Update(&s_button_under_test, BTN_STATE_ACTIVE);
		}
		
		CPPUNIT_ASSERT_EQUAL(0, repeatCount);
		CPPUNIT_ASSERT_EQUAL(1, changeStateCount);
		
		/* One more count should hit repeat */
		BTN_Update(&s_button_under_test, BTN_STATE_ACTIVE);
		
		CPPUNIT_ASSERT_EQUAL(1, repeatCount);
		CPPUNIT_ASSERT_EQUAL(1, changeStateCount);
		
		/* Keep button held for ages */
		for (i = 0; i < 1000; i++)
		{
			BTN_Update(&s_button_under_test, BTN_STATE_ACTIVE);
		}
		
		CPPUNIT_ASSERT_EQUAL(11, repeatCount);
		CPPUNIT_ASSERT_EQUAL(1, changeStateCount);
		
		/* Let go of button for 99 counts */
		for (i = 0; i < 99; i++)
		{
			BTN_Update(&s_button_under_test, BTN_STATE_INACTIVE);
		}
		
		CPPUNIT_ASSERT_EQUAL(11, repeatCount);
		CPPUNIT_ASSERT_EQUAL(1, changeStateCount);
		
		/* One more count should hit change state */
		BTN_Update(&s_button_under_test, BTN_STATE_INACTIVE);
		
		CPPUNIT_ASSERT_EQUAL(0, repeatCount);
		CPPUNIT_ASSERT_EQUAL(2, changeStateCount);
		
	}

	void test_UpdateWithDebouncing(void)
	{

		int i;
		
		/* Test the debouncing */
		for (i = 0; i < 50; i++)
		{
			BTN_Update(&s_button_under_test, BTN_STATE_ACTIVE);
		}
		
		CPPUNIT_ASSERT_EQUAL(0, repeatCount);
		CPPUNIT_ASSERT_EQUAL(0, changeStateCount);
		CPPUNIT_ASSERT_EQUAL(50, (int)s_button_under_test.debounce_count);
		
		for (i = 0; i < 120; i++)
		{
			BTN_Update(&s_button_under_test, BTN_STATE_INACTIVE);
		}
		
		CPPUNIT_ASSERT_EQUAL(0, repeatCount);
		CPPUNIT_ASSERT_EQUAL(0, changeStateCount);
		CPPUNIT_ASSERT_EQUAL(0, (int)s_button_under_test.debounce_count);
		
		for (i = 0; i < 50; i++)
		{
			BTN_Update(&s_button_under_test, BTN_STATE_ACTIVE);
		}
		
		CPPUNIT_ASSERT_EQUAL(0, repeatCount);
		CPPUNIT_ASSERT_EQUAL(0, changeStateCount);
		CPPUNIT_ASSERT_EQUAL(50, (int)s_button_under_test.debounce_count);
		
		for (i = 0; i < 90; i++)
		{
			BTN_Update(&s_button_under_test, BTN_STATE_ACTIVE);
		}
		
		CPPUNIT_ASSERT_EQUAL(0, repeatCount);
		CPPUNIT_ASSERT_EQUAL(1, changeStateCount);
		CPPUNIT_ASSERT_EQUAL(s_button_under_test.max_debounce_count, s_button_under_test.debounce_count);
		
		for (i = 0; i < 10; i++)
		{
			BTN_Update(&s_button_under_test, BTN_STATE_INACTIVE);
		}
		
		CPPUNIT_ASSERT_EQUAL(0, repeatCount);
		CPPUNIT_ASSERT_EQUAL(1, changeStateCount);
		CPPUNIT_ASSERT_EQUAL(90, (int)s_button_under_test.debounce_count);
		
		for (i = 0; i < 100; i++)
		{
			BTN_Update(&s_button_under_test, BTN_STATE_INACTIVE);
		}
		
		CPPUNIT_ASSERT_EQUAL(0, repeatCount);
		CPPUNIT_ASSERT_EQUAL(2, changeStateCount);
		CPPUNIT_ASSERT_EQUAL(0, (int)s_button_under_test.debounce_count);
	}

	void test_NoRepeatInit(void)
	{
		s_button_under_test.repeat_callback = NULL;
		*(uint16_t*)(&s_button_under_test.max_repeat_count) = 1; // Pointer abuse to get round read-only variable
		CPPUNIT_ASSERT_EQUAL(false, BTN_InitHandler(&s_button_under_test));
		
		s_button_under_test.repeat_callback = repeatCallback;
		*(uint16_t*)(&s_button_under_test.max_repeat_count) = 0;
		CPPUNIT_ASSERT_EQUAL(false, BTN_InitHandler(&s_button_under_test));
		
		s_button_under_test.repeat_callback = NULL;
		*(uint16_t*)(&s_button_under_test.max_repeat_count) = 0;
		CPPUNIT_ASSERT_EQUAL(true, BTN_InitHandler(&s_button_under_test));	
	}

	void test_NoRepeatOnlyInvokesCallbacksOnce(void)
	{
		// Clear the repeat variables before test
		s_button_under_test.repeat_callback = NULL;
		*(uint16_t*)(&s_button_under_test.max_repeat_count) = 0;
		
		//Throw in lots of active presses, should get no repeats and one change
		for (int i = 0; i < 500; ++i)
		{
			BTN_Update(&s_button_under_test, BTN_STATE_ACTIVE);
		}
		
		CPPUNIT_ASSERT_EQUAL(0, repeatCount);
		CPPUNIT_ASSERT_EQUAL(1, changeStateCount);
		
	}
};

int main()
{
   CppUnit::TextUi::TestRunner runner;
   
   CPPUNIT_TEST_SUITE_REGISTRATION( ButtonTest );

   CppUnit::TestFactoryRegistry &registry = CppUnit::TestFactoryRegistry::getRegistry();

   runner.addTest( registry.makeTest() );
   runner.run();

   return 0;
}
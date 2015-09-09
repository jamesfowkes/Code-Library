#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "unity.h"

#include "button.h"

void repeatCallback(void);
void changeStateCallback(BTN_STATE_ENUM state);

BTN button = {
	BTN_STATE_INACTIVE,
	changeStateCallback,
	repeatCallback,
	0,
	100,
	0,
	100,
};

BTN bad_button_1 = {
	BTN_STATE_INACTIVE,
	NULL,
	NULL,
	0,
	0,
	0,
	0,
};

int repeatCount = 0;
int changeStateCount = 0;

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
	memcpy(&button, &good_button, sizeof(BTN));
	
	repeatCount = 0;
	changeStateCount = 0;
}

void tearDown(void)
{

}

void test_InitHandler(void)
{
	TEST_ASSERT_EQUAL(true, BTN_InitHandler(&button));
	
	// Get a non-const pointer to the button integers so we can tweak the constant values for testing
	uint16_t * pmax_repeat_count = (uint16_t *)(&button.max_repeat_count);
	uint16_t * pmax_debounce_count = (uint16_t *)(&button.max_debounce_count);
	
	*pmax_repeat_count = 0;
	*pmax_debounce_count = 100;
	TEST_ASSERT_EQUAL(false, BTN_InitHandler(&button));
	
	*pmax_repeat_count = 100;
	*pmax_debounce_count = 0;
	TEST_ASSERT_EQUAL(false, BTN_InitHandler(&button));
}

void test_Update(void)
{
	BTN_Update(&button, BTN_STATE_INACTIVE);
	
	TEST_ASSERT_EQUAL(0, repeatCount);
	TEST_ASSERT_EQUAL(0, changeStateCount);
	
	int i;
	
	/* No button press */
	for (i = 0; i < 200; i++)
	{
		BTN_Update(&button, BTN_STATE_INACTIVE);
	}
	
	TEST_ASSERT_EQUAL(0, repeatCount);
	TEST_ASSERT_EQUAL(0, changeStateCount);
	
	/* Active button press up to 99*/
	for (i = 0; i < 99; i++)
	{
		BTN_Update(&button, BTN_STATE_ACTIVE);
	}
	
	TEST_ASSERT_EQUAL(0, repeatCount);
	TEST_ASSERT_EQUAL(0, changeStateCount);
	
	/* One more press changes state*/
	BTN_Update(&button, BTN_STATE_ACTIVE);
	
	TEST_ASSERT_EQUAL(0, repeatCount);
	TEST_ASSERT_EQUAL(1, changeStateCount);
	
	/* Continue in active state for 99 counts */
	for (i = 0; i < 99; i++)
	{
		BTN_Update(&button, BTN_STATE_ACTIVE);
	}
	
	TEST_ASSERT_EQUAL(0, repeatCount);
	TEST_ASSERT_EQUAL(1, changeStateCount);
	
	/* One more count should hit repeat */
	BTN_Update(&button, BTN_STATE_ACTIVE);
	
	TEST_ASSERT_EQUAL(1, repeatCount);
	TEST_ASSERT_EQUAL(1, changeStateCount);
	
	/* Keep button held for ages */
	for (i = 0; i < 1000; i++)
	{
		BTN_Update(&button, BTN_STATE_ACTIVE);
	}
	
	TEST_ASSERT_EQUAL(11, repeatCount);
	TEST_ASSERT_EQUAL(1, changeStateCount);
	
	/* Let go of button for 99 counts */
	for (i = 0; i < 99; i++)
	{
		BTN_Update(&button, BTN_STATE_INACTIVE);
	}
	
	TEST_ASSERT_EQUAL(11, repeatCount);
	TEST_ASSERT_EQUAL(1, changeStateCount);
	
	/* One more count should hit change state */
	BTN_Update(&button, BTN_STATE_INACTIVE);
	
	TEST_ASSERT_EQUAL(0, repeatCount);
	TEST_ASSERT_EQUAL(2, changeStateCount);
	
}

void test_UpdateWithDebouncing(void)
{

	int i;
	
	/* Test the debouncing */
	for (i = 0; i < 50; i++)
	{
		BTN_Update(&button, BTN_STATE_ACTIVE);
	}
	
	TEST_ASSERT_EQUAL(0, repeatCount);
	TEST_ASSERT_EQUAL(0, changeStateCount);
	TEST_ASSERT_EQUAL(50, button.debounce_count);
	
	for (i = 0; i < 120; i++)
	{
		BTN_Update(&button, BTN_STATE_INACTIVE);
	}
	
	TEST_ASSERT_EQUAL(0, repeatCount);
	TEST_ASSERT_EQUAL(0, changeStateCount);
	TEST_ASSERT_EQUAL(0, button.debounce_count);
	
	for (i = 0; i < 50; i++)
	{
		BTN_Update(&button, BTN_STATE_ACTIVE);
	}
	
	TEST_ASSERT_EQUAL(0, repeatCount);
	TEST_ASSERT_EQUAL(0, changeStateCount);
	TEST_ASSERT_EQUAL(50, button.debounce_count);
	
	for (i = 0; i < 90; i++)
	{
		BTN_Update(&button, BTN_STATE_ACTIVE);
	}
	
	TEST_ASSERT_EQUAL(0, repeatCount);
	TEST_ASSERT_EQUAL(1, changeStateCount);
	TEST_ASSERT_EQUAL(button.max_debounce_count, button.debounce_count);
	
	for (i = 0; i < 10; i++)
	{
		BTN_Update(&button, BTN_STATE_INACTIVE);
	}
	
	TEST_ASSERT_EQUAL(0, repeatCount);
	TEST_ASSERT_EQUAL(1, changeStateCount);
	TEST_ASSERT_EQUAL(90, button.debounce_count);
	
	for (i = 0; i < 100; i++)
	{
		BTN_Update(&button, BTN_STATE_INACTIVE);
	}
	
	TEST_ASSERT_EQUAL(0, repeatCount);
	TEST_ASSERT_EQUAL(2, changeStateCount);
	TEST_ASSERT_EQUAL(0, button.debounce_count);
}

void test_NoRepeatInit(void)
{
	button.repeat_callback = NULL;
	*(uint16_t*)(&button.max_repeat_count) = 1; // Pointer abuse to get round read-only variable
	TEST_ASSERT_EQUAL(false, BTN_InitHandler(&button));
	
	button.repeat_callback = repeatCallback;
	*(uint16_t*)(&button.max_repeat_count) = 0;
	TEST_ASSERT_EQUAL(false, BTN_InitHandler(&button));
	
	button.repeat_callback = NULL;
	*(uint16_t*)(&button.max_repeat_count) = 0;
	TEST_ASSERT_EQUAL(true, BTN_InitHandler(&button));	
}

void test_NoRepeatOnlyInvokesCallbacksOnce(void)
{
	// Clear the repeat variables before test
	button.repeat_callback = NULL;
	*(uint16_t*)(&button.max_repeat_count) = 0;
	
	//Throw in lots of active presses, should get no repeats and one change
	for (int i = 0; i < 500; ++i)
	{
		BTN_Update(&button, BTN_STATE_ACTIVE);
	}
	
	TEST_ASSERT_EQUAL(0, repeatCount);
	TEST_ASSERT_EQUAL(1, changeStateCount);
	
}

void repeatCallback(void)
{
	repeatCount++;
}

void changeStateCallback(BTN_STATE_ENUM state)
{
	(void)state;
	repeatCount = 0;
	changeStateCount++;
}

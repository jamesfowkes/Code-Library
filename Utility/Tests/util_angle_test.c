#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

#include "unity.h"

#include "../util_angle.h"

char msgbuf[20];

void setUp(void)
{

}

void tearDown(void)
{

}

void test_integerDegrees(void)
{
	int16_t a = 0;
	uint16_t result = 0;
	uint16_t expected_result = 180;
	for (a = -720; a < 720; a++)
	{
		sprintf(msgbuf, "Angle = %d", a);
		result = reciprocal_deg(a);
		TEST_ASSERT_EQUAL_MESSAGE(expected_result, result, msgbuf);
		expected_result++;
		expected_result = expected_result % 360;
	}
}

void test_tenthsDegrees(void)
{
	int16_t a = 0;
	uint16_t result = 0;
	uint16_t expected_result = 1800;
	for (a = -7200; a < 7200; a++)
	{
		sprintf(msgbuf, "Angle = %d", a);
		result = reciprocal_tdeg(a);
		TEST_ASSERT_EQUAL_MESSAGE(expected_result, result, msgbuf);
		expected_result++;
		expected_result = expected_result % 3600;
	}
}

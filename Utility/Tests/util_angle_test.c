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

void test_recipIntegerDegrees(void)
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

void test_recipTenthsDegrees(void)
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

void test_mirrorIntegerDegreesAboutZero(void)
{
	TEST_ASSERT_EQUAL(0, mirror_deg(0, 0));
	TEST_ASSERT_EQUAL(315, mirror_deg(45, 0));
	TEST_ASSERT_EQUAL(270, mirror_deg(90, 0));
	TEST_ASSERT_EQUAL(225, mirror_deg(135, 0));
	TEST_ASSERT_EQUAL(180, mirror_deg(180, 0));
	TEST_ASSERT_EQUAL(135, mirror_deg(225, 0));
	TEST_ASSERT_EQUAL(90, mirror_deg(270, 0));
	TEST_ASSERT_EQUAL(45, mirror_deg(315, 0));
	TEST_ASSERT_EQUAL(0, mirror_deg(360, 0));
}

void test_mirrorIntegerDegreesAboutNonZero(void)
{
	uint16_t mirror_angle = 37;
	TEST_ASSERT_EQUAL(0 + mirror_angle, mirror_deg(0, mirror_angle));
	TEST_ASSERT_EQUAL(315 + mirror_angle, mirror_deg(45, mirror_angle));
	TEST_ASSERT_EQUAL(270 + mirror_angle, mirror_deg(90, mirror_angle));
	TEST_ASSERT_EQUAL(225 + mirror_angle, mirror_deg(135, mirror_angle));
	TEST_ASSERT_EQUAL(180 + mirror_angle, mirror_deg(180, mirror_angle));
	TEST_ASSERT_EQUAL(135 + mirror_angle, mirror_deg(225, mirror_angle));
	TEST_ASSERT_EQUAL(90 + mirror_angle, mirror_deg(270, mirror_angle));
	TEST_ASSERT_EQUAL(45 + mirror_angle, mirror_deg(315, mirror_angle));
	TEST_ASSERT_EQUAL(0 + mirror_angle, mirror_deg(360, mirror_angle));
}

void test_mirrorTenthDegreesAboutZero(void)
{
	TEST_ASSERT_EQUAL(0, mirror_tdeg(0, 0));
	TEST_ASSERT_EQUAL(3150, mirror_tdeg(450, 0));
	TEST_ASSERT_EQUAL(2700, mirror_tdeg(900, 0));
	TEST_ASSERT_EQUAL(2250, mirror_tdeg(1350, 0));
	TEST_ASSERT_EQUAL(1800, mirror_tdeg(1800, 0));
	TEST_ASSERT_EQUAL(1350, mirror_tdeg(2250, 0));
	TEST_ASSERT_EQUAL(900, mirror_tdeg(2700, 0));
	TEST_ASSERT_EQUAL(450, mirror_tdeg(3150, 0));
	TEST_ASSERT_EQUAL(0, mirror_tdeg(3600, 0));
}

void test_mirrorTenthDegreesAboutNonZero(void)
{
	uint16_t mirror_angle = 370;
	TEST_ASSERT_EQUAL(0 + mirror_angle, mirror_tdeg(0, mirror_angle));
	TEST_ASSERT_EQUAL(3150 + mirror_angle, mirror_tdeg(450, mirror_angle));
	TEST_ASSERT_EQUAL(2700 + mirror_angle, mirror_tdeg(900, mirror_angle));
	TEST_ASSERT_EQUAL(2250 + mirror_angle, mirror_tdeg(1350, mirror_angle));
	TEST_ASSERT_EQUAL(1800 + mirror_angle, mirror_tdeg(1800, mirror_angle));
	TEST_ASSERT_EQUAL(1350 + mirror_angle, mirror_tdeg(2250, mirror_angle));
	TEST_ASSERT_EQUAL(900 + mirror_angle, mirror_tdeg(2700, mirror_angle));
	TEST_ASSERT_EQUAL(450 + mirror_angle, mirror_tdeg(3150, mirror_angle));
	TEST_ASSERT_EQUAL(0 + mirror_angle, mirror_tdeg(3600, mirror_angle));
}

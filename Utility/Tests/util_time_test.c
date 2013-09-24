#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

#include "unity.h"

#include "../util_time.h"

static char message[100];

static char * wkdy[] = {
	"Sun",
	"Mon",
	"Tue",
	"Wed",
	"Thu",
	"Fri",
	"Sat"
};

void setUp(void)
{

}

void tearDown(void)
{

}

test_LeapYears(void)
{
	TEST_ASSERT_EQUAL(is_leap_year(1970), false);
	TEST_ASSERT_EQUAL(is_leap_year(1971), false);
	TEST_ASSERT_EQUAL(is_leap_year(1972), true);
	TEST_ASSERT_EQUAL(is_leap_year(1973), false);
}

test_DaysInMonth(void)
{
	TEST_ASSERT_EQUAL(days_in_month(1, true), 29);
	TEST_ASSERT_EQUAL(days_in_month(1, false), 28);
}

test_UnixSecondsToTime(void)
{
	UNIX_TIMESTAMP seconds = 0;
	TM time;
	int m = 0;
	int h = 0;
	int dd = 1;
	int mm = 0;
	int yy = 70;
	int daycount = 0;
	int yday = 0;
	
	bool bLeapYear = is_leap_year(1970);
	char * pMessage = message;
	
	do
	{
		unix_seconds_to_time(seconds, &time);
		
		pMessage = message;
		pMessage += sprintf(pMessage, "%lu - %s ", seconds, wkdy[time.tm_wday]);
		pMessage += sprintf(pMessage, "%d/%d/%d ", time.tm_mday, time.tm_mon + 1, TWO_DIGIT_YEAR(time.tm_year));
		
		if (dd > days_in_month(mm, bLeapYear))
		{
			dd = 1;
			++mm;
			if (mm == (DEC+1))
			{
				mm = JAN;
				++yy;
				yday = 0;
				bLeapYear = is_leap_year(yy+1900);
			}
		}
		
		TEST_ASSERT_EQUAL_MESSAGE(dd, time.tm_mday, message);
		TEST_ASSERT_EQUAL_MESSAGE(mm, time.tm_mon, message);
		TEST_ASSERT_EQUAL_MESSAGE(yy, time.tm_year, message);
		TEST_ASSERT_EQUAL_MESSAGE(yday, time.tm_yday, message);
		
		++dd;
		++yday;
		
	} while ((seconds += 86400) < 1379969817);
}

test_TimeToUnixSeconds(void)
{
	UNIX_TIMESTAMP seconds = 0;
	TM time;
	int m = 0;
	int h = 0;
	int dd = 1;
	int mm = 0;
	int yy = 70;

	bool bLeapYear = is_leap_year(1970);
	char * pMessage = message;
	
	do
	{
		unix_seconds_to_time(seconds, &time);
		
		pMessage = message;
		pMessage += sprintf(pMessage, "%lu - %s ", seconds, wkdy[time.tm_wday]);
		pMessage += sprintf(pMessage, "%d/%d/%d (%d)", time.tm_mday, time.tm_mon + 1, TWO_DIGIT_YEAR(time.tm_year), time.tm_yday);
		
		if (dd > days_in_month(mm, bLeapYear))
		{
			dd = 1;
			++mm;
			if (mm == (DEC+1))
			{
				mm = JAN;
				++yy;
				bLeapYear = is_leap_year(yy+1900);
			}
		}

		TEST_ASSERT_EQUAL_MESSAGE(seconds, time_to_unix_seconds(&time), message);
		++dd;
	} while ((seconds += 86400) < 1379969817);
}

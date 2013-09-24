#include <stdbool.h>
#include <stdint.h>

#include "util_time.h"

// For non-leap years ONLY!
static const uint16_t s_days_in_month[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

// For non-leap years ONLY!
static const uint16_t s_days_into_year[] = {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};

bool is_leap_year(uint16_t year)
{
	return ((year & 3) == 0 && ((year % 25) != 0 || (year & 15) == 0));
}

uint16_t days_in_month(uint8_t month, bool is_leap_year)
{
	if ((month == 1) && is_leap_year)
	{
		return 29;
	}
	else
	{
		return s_days_in_month[month];
	}
}

uint16_t get_year_days(const TM * tm)
{
	uint16_t days = s_days_into_year[tm->tm_mon] + tm->tm_mday;

	if (is_leap_year(tm->tm_year + 1900)) { days++; }

	return days;
}

void unix_seconds_to_time(UNIX_TIMESTAMP sec, TM * tm)
{
	uint16_t day;
	uint16_t year;
	uint16_t days_in_year;
	uint8_t month;
	
	bool bFoundYear = false;
	bool bIsLeapYear = false;
	
	tm->tm_sec = sec % 60;
	sec /= 60;
	tm->tm_min = sec % 60;
	sec /= 60;
	tm->tm_hour = sec % 24;
	day = sec / 24;

	tm->tm_wday = (day + FIRST_DAY_OF_WEEK_1_JAN_1970) % 7; // weekday

	year = FIRST_UNIX_YEAR;
	
	day--;
	while (!bFoundYear)
	{
		bIsLeapYear = is_leap_year(year);
		days_in_year = bIsLeapYear ? 366 : 365;
		
		bFoundYear = day < days_in_year;
		
		if (!bFoundYear)
		{
			day -= days_in_year;
			year++;
		}
	}
	
	tm->tm_yday = day;
	
	/* Convert from year to C year (offset to 1900) */
	tm->tm_year = year - 1900;
	
	for(month = 0; day >= days_in_month(month, bIsLeapYear); month++ )
	{
		day -= days_in_month(month, bIsLeapYear);
	}
	
	tm->tm_mon = month + 1; // 1..12
	tm->tm_mday = day + 1; // 1..31
}

UNIX_TIMESTAMP time_to_unix_seconds(TM * tm)
{
	UNIX_TIMESTAMP secs = 0;
	
	if (tm->tm_year < 70) { return 0; }
	
	uint32_t days = tm->tm_yday;
	uint16_t year = tm->tm_year + FIRST_C_YEAR - FIRST_UNIX_YEAR;

	while (year--)
	{
		days += is_leap_year(year + FIRST_UNIX_YEAR) ? 366 : 365;
	}
	
	days++;
	
	secs += (days * 86400);

	secs += (tm->tm_hour * 3600);
	secs += (tm->tm_min * 60);
	secs += tm->tm_sec;
	
	return secs;
}

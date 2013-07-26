#include <stdbool.h>
#include <stdint.h>

#include "util_time.h"

bool is_leap_year(uint16_t year)
{
	bool is_leap_year = false;

	if ((year % 400)  == 0)
	{
		is_leap_year = true;
	}
	else if ((year % 100) == 0)
	{
		is_leap_year = false;
	}
	else if ((year % 4) == 0)
	{
		is_leap_year = true;
	}

	return is_leap_year;
}

int get_year_days(const TM * tm)
{
	int days_into_year[] = {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};

	int days = days_into_year[tm->tm_mon] + tm->tm_mday;

	if (is_leap_year(tm->tm_year + 1900)) { days++; }

	return days;
}

uint32_t years_to_seconds(uint16_t start_year, uint8_t years)
{
	uint32_t seconds = 0;

	while (years--)
	{
		seconds += is_leap_year(start_year) ? (84600 * 366) :(84600 * 365);
		start_year++;
	}

	return seconds;
}

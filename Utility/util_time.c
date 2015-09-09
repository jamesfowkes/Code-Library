#include <stdbool.h>
#include <stdint.h>

#include "util_time.h"
#include "util_simple_compare.h"
#include "util_macros.h"

// For non-leap years ONLY!
static const uint16_t s_days_in_month[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

// For non-leap years ONLY!
static const uint16_t s_days_into_year[] = {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};

bool is_leap_year(GREGORIAN_YEAR year)
{
	return ((year & 3) == 0 && ((year % 25) != 0 || (year & 15) == 0));
}

uint16_t days_in_month(uint8_t month, bool is_leap_year)
{
	if ((month == FEB) && is_leap_year)
	{
		return 29;
	}
	else if (month > DEC)
	{
		return 0;
	}
	else
	{
		return s_days_in_month[month];
	}
}

uint16_t calculate_days_into_year(const TM * tm)
{
	uint16_t days = s_days_into_year[tm->tm_mon] + tm->tm_mday;

	if (is_leap_year(tm->tm_year + 1900))
	{
		if (days > FEB28TH_DAYS_INTO_YEAR)
		{
			days++; //There's an extra day (Feb 29th) not accounted for months days array
		}
	}

	days--; // Subtract 1 because tm->tm_mday is 1-31, and first day of year is 0th day
	return days;
}

void unix_seconds_to_time(UNIX_TIMESTAMP sec, TM * tm)
{
	uint16_t day;
	GREGORIAN_YEAR year;
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

	year = FIRST_UNIX_YEAR_GR;
	
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
	
	tm->tm_mon = month; // 0..11
	tm->tm_mday = day + 1; // 1..31
}

UNIX_TIMESTAMP time_to_unix_seconds(TM const * const tm)
{
	UNIX_TIMESTAMP secs = 0;
	
	if (tm->tm_year < FIRST_UNIX_YEAR_C) { return 0; }
	
	uint32_t days = tm->tm_yday;
	
	// Convert number of years from the UNIX epoch to specified year
	C_STRUCT_YEAR year = tm->tm_year;

	while (year-- > FIRST_UNIX_YEAR_C)
	{
		days += is_leap_year(C_TO_GREGORIAN_YEAR(year)) ? 366 : 365;
	}
	
	secs += (days * 86400UL);

	secs += (tm->tm_hour * 3600UL);
	secs += (tm->tm_min * 60UL);
	secs += tm->tm_sec;
	
	return secs;
}

void time_increment_seconds(TM * tm)
{
	if (!tm) { return; }
	
	incrementwithrollover(tm->tm_sec, 59);
	
	if (tm->tm_sec == 0)
	{
		incrementwithrollover(tm->tm_min, 59);
	}
	else
	{
		return;
	}
	
	if (tm->tm_min == 0)
	{
		incrementwithrollover(tm->tm_hour, 23);
	}
	else
	{
		return;
	}
	
	bool bIsLeapYear = is_leap_year(tm->tm_year);
	
	if (tm->tm_hour == 0)
	{
		incrementwithrollover(tm->tm_wday, (int)SAT);
		incrementwithrollover(tm->tm_mday, (int)days_in_month(tm->tm_mon, bIsLeapYear));
		incrementwithrollover(tm->tm_yday, bIsLeapYear ? 365 : 364);
	}
	else
	{
		return;
	}
	
	if (tm->tm_mday == 0)
	{
		tm->tm_mday = 1; // Month starts at 1, not 0
		incrementwithrollover(tm->tm_mon, 11);
	}
	else
	{
		return;
	}	
}

void time_cpy(TM* dst, TM* src)
{
	if (dst && src)
	{
		dst->tm_year = src->tm_year;
		dst->tm_mon = src->tm_mon;
		dst->tm_mday = src->tm_mday;
		dst->tm_hour = src->tm_hour;
		dst->tm_min = src->tm_min;
		dst->tm_sec = src->tm_sec;
		dst->tm_wday = src->tm_wday;
	}
}

bool days_in_month_valid(int days, uint8_t month, int year)
{
	// Validate month, date, hour, minute and second
    GREGORIAN_YEAR fourDigitYear = 2000 + year;
    int _days_in_month = days_in_month(month, is_leap_year(fourDigitYear));
    return (days > 0) && (days <= _days_in_month); // Date between 1 and <days in month>
}

/*
 * time_to_datetime_string
 *
 * Simple conversion from time struct to 
 * time string in the format YY-MM-DD DDD HH:MM:SS
 */

bool time_to_datetime_string(TM* pTime, DT_FORMAT_STRING * dt_string)
{
    if (!pTime) { return false; }
    if (!dt_string) { return false; }

    dt_string->hyphen1 = '-';
    dt_string->hyphen2 = '-';
    dt_string->space1 = ' ';
    dt_string->space2 = ' ';
    dt_string->colon1 = ':';
    dt_string->colon2 = ':';

    weekday_to_chars(pTime->tm_wday, dt_string->day);

    dt_string->year[0] = (pTime->tm_year / 10) + '0';
    dt_string->year[1] = (pTime->tm_year % 10) + '0';

    dt_string->month[0] = (pTime->tm_mon / 10) + '0';
    dt_string->month[1] = (pTime->tm_mon % 10) + '0';

    dt_string->date[0] = (pTime->tm_mday / 10) + '0';
    dt_string->date[1] = (pTime->tm_mday % 10) + '0';

    dt_string->hour[0] = (pTime->tm_hour / 10) + '0';
    dt_string->hour[1] = (pTime->tm_hour % 10) + '0';

    dt_string->minute[0] = (pTime->tm_min / 10) + '0';
    dt_string->minute[1] = (pTime->tm_min % 10) + '0';

    dt_string->second[0] = (pTime->tm_sec / 10) + '0';
    dt_string->second[1] = (pTime->tm_sec % 10) + '0';

    return true;
}

/*
 * chars_to_weekday
 *
 * Takes a three-letter day string in three_char_day and returns 0-6 for the day
 * of the week, where SUN = 0;
 */
bool chars_to_weekday(int * pResult, char * three_char_day)
{
    // Sets pResult to 0 for SUN, 1 for MON etc. Returns false if not a day string.
    if (!pResult) { return false; }

    if (simple_cmp(3, three_char_day, "SUN")) { *pResult = 0; return true; }
    if (simple_cmp(3, three_char_day, "MON")) { *pResult = 1; return true; }
    if (simple_cmp(3, three_char_day, "TUE")) { *pResult = 2; return true; }
    if (simple_cmp(3, three_char_day, "WED")) { *pResult = 3; return true; }
    if (simple_cmp(3, three_char_day, "THU")) { *pResult = 4; return true; }
    if (simple_cmp(3, three_char_day, "FRI")) { *pResult = 5; return true; }
    if (simple_cmp(3, three_char_day, "SAT")) { *pResult = 6; return true; }

    return false;
}

/*
 * weekday_to_chars
 *
 * Takes an int for the weekday (0-6, SUN  0) and fills str with
 * three-letter day string for that day.
 */
void weekday_to_chars(int wday, char * str)
{
    // Set str to SUN for 0, MON for 1 etc...
    switch(wday)
    {
    case 0:
        str[0] = 'S'; str[1] = 'U'; str[2] = 'N';
        break;
    case 1:
        str[0] = 'M'; str[1] = 'O'; str[2] = 'N';
        break;
    case 2:
        str[0] = 'T'; str[1] = 'U'; str[2] = 'E';
        break;
    case 3:
        str[0] = 'W'; str[1] = 'E'; str[2] = 'D';
        break;
    case 4:
        str[0] = 'T'; str[1] = 'H'; str[2] = 'U';
        break;
    case 5:
        str[0] = 'F'; str[1] = 'R'; str[2] = 'I';
        break;
    case 6:
        str[0] = 'S'; str[1] = 'A'; str[2] = 'T';
        break;
 	default:
 		break;
    }
}
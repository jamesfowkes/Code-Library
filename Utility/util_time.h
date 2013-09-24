#ifndef _UTIL_TIME_H_
#define _UTIL_TIME_H_

/*
 * This struct taken from C library time.h
 */
struct tm
{
	int tm_sec;    /* seconds after the minute (0 to 61) */
	int tm_min;    /* minutes after the hour (0 to 59) */
	int tm_hour;   /* hours since midnight (0 to 23) */
	int tm_mday;   /* day of the month (1 to 31) */
	int tm_mon;    /* months since January (0 to 11) */
	int tm_year;   /* years since 1900 */
	int tm_wday;   /* days since Sunday (0 to 6 Sunday=0) */
	int tm_yday;   /* days since January 1 (0 to 365) */
	int tm_isdst;  /* Daylight Savings Time */
};
typedef struct tm TM;

typedef uint32_t UNIX_TIMESTAMP;

bool is_leap_year(uint16_t year);
uint16_t days_in_month(uint8_t month, bool is_leap_year);
uint16_t get_year_days(const TM * tm);

void unix_seconds_to_time(UNIX_TIMESTAMP sec, TM * tm);
UNIX_TIMESTAMP time_to_unix_seconds(TM * tm);

#define FIRST_UNIX_YEAR (1970)
#define FIRST_DAY_OF_WEEK_1_JAN_1970 (4) // Thursday
#define FEB_28 (58) // Feb 28th is 58th day of year
#define FIRST_C_YEAR (1900)

#define TWO_DIGIT_YEAR(year) (year % 100)

#define MS_PER_S (1000)
#define S_PER_MIN (60)
#define MIN_PER_HOUR (60)

#define S_PER_HOUR (S_PER_MIN * MIN_PER_HOUR)
#define MS_PER_MIN (MS_PER_S * S_PER_MIN)
#define MS_PER_HOUR (MS_PER_S * S_PER_HOUR)

#define MINS_AND_SECS_TO_SECS(m, s) ((m*60) + s)

#endif

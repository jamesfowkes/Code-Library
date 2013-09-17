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

bool is_leap_year(uint16_t year);
int get_year_days(const TM * tm);
uint32_t years_to_seconds(uint16_t start_year, uint8_t years);

#define MS_PER_S (1000)
#define S_PER_MIN (60)
#define MIN_PER_HOUR (60)

#define S_PER_HOUR (S_PER_MIN * MIN_PER_HOUR)
#define MS_PER_MIN (MS_PER_S * S_PER_MIN)
#define MS_PER_HOUR (MS_PER_S * S_PER_HOUR)

#define MINS_AND_SECS_TO_SECS(m, s) ((m*60) + s)

#endif

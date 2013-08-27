/*
 * Standard library includes
 */

#include <stdbool.h>
#include <stdint.h>

/*
 * Utility Library Includes
 */

#include "util_time.h"
#include "util_bcd.h"

/*
 * Device Library Includes
 */

#include "lib_ds3231.h"

/*
 * Private defines and typedefs
 */

#define	AMPM_SELECT		(1 << 6)
#define	AMPM_SET		(1 << 5)

#define	CENTURY_SELECT	(1 << 7)

#define	A1IE			(1 << 0)
#define	A2IE			(1 << 1)
#define	INTCN			(1 << 2)
#define RS1_BIT			(3)
#define	RS1				(1 << RS1_BIT)
#define	RS2				(1 << 4)
#define	CONV			(1 << 5)
#define	BBSWQ			(1 << 6)
#define	EOSC			(1 << 7)

#define	A1F				(1 << 0)
#define	A2F				(1 << 1)
#define	BSY				(1 << 2)
#define	EN32kHz			(1 << 3)
#define	OSF				(1 << 7)


#define	REG_SECS		0
#define	REG_MINS		1
#define	REG_HRS			2
#define	REG_DAY			3
#define	REG_DATE		4
#define	REG_MONTH		5
#define REG_YEAR		6
#define	REG_ALRM1_SECS	7
#define	REG_ALRM1_MINS	8
#define	REG_ALRM1_HRS	9
#define	REG_ALRM1_DATE	10
#define	REG_ALRM2_MINS	11
#define	REG_ALRM2_HRS	12
#define	REG_ALRM2_DATE	13
#define	REG_CONTROL		14
#define	REG_STATUS		15
#define	REG_AGING		16
#define	REG_TEMPMSB		17
#define	REG_TEMPLSB		18

struct time_registers
{
	uint8_t seconds;
	uint8_t minutes;
	uint8_t hours;
};
typedef struct time_registers TIME_REGISTERS;

struct date_registers
{
	uint8_t day;
	uint8_t date;
	uint8_t month;
	uint8_t year;
};
typedef struct date_registers DATE_REGISTERS;

struct alarm_registers
{
	uint8_t seconds;
	uint8_t minutes;
	uint8_t hours;
	uint8_t day_date;
};
typedef struct alarm_registers ALARM_REGISTERS;

struct control_registers
{
	uint8_t control;
	uint8_t status;
	uint8_t aging;
	uint8_t temp_msb;
	uint8_t temp_lsb;
};
typedef struct control_registers CONTROL_REGISTERS;

enum ds3231_rate_enum
{
	DS3131_RATE_1HZ,
	DS3131_RATE_1024HZ,
	DS3131_RATE_4096HZ,
	DS3131_RATE_8191HZ,
};
typedef enum ds3231_rate_enum DS3231_RATE_ENUM;

enum ds3231_time_format_enum
{
	DS3231_TIME_FORMAT_AMPM,
	DS3231_TIME_FORMAT_24HR,
};
typedef enum ds3231_time_format_enum DS3231_TIME_FORMAT_ENUM;

enum ds3231_date_type_enum
{
	DS3232_DATE_TYPE_DAYOFMONTH,
	DS3232_DATE_TYPE_DAYOFWEEK
};
typedef enum ds3231_date_type_enum DS3231_DATE_TYPE_ENUM;

enum ds3231_alarm_rpt_enum
{
	DS3231_ALARM_RPT_EVERY_S,
	DS3231_ALARM_RPT_EVERY_M,
	DS3231_ALARM_RPT_MATCH_S,
	DS3231_ALARM_RPT_MATCH_MS,
	DS3231_ALARM_RPT_MATCH_HMS,
	DS3231_ALARM_RPT_MATCH_DHMS

};
typedef enum ds3231_alarm_rpt_enum DS3231_ALARM_RPT_ENUM;

enum ds3231_alarm_enum
{
	DS3231_ALARM_1,
	DS3231_ALARM_2
};
typedef enum ds3231_alarm_enum DS3231_ALARM_ENUM;

/*
 * Private Variables
 */

static I2C_WR_FN wr_fn;
static I2C_RD_FN rd_fn;

uint8_t control;
uint8_t status;

/*
 * Private Function Prototypes
 */

static char to_bcd(int from);
static int from_bcd(int bcd);

/*
 * Public Functions
 */

bool DS3231_Init(I2C_WR_FN _wr_fn, I2C_WR_FN _rd_fn)
{
	wr_fn = _wr_fn;
	rd_fn =_rd_fn;

	return (wr_fn && rd_fn);
}

bool DS3231_SetTime(const TM * tm, bool ampm_mode)
{
	TIME_REGISTERS time;
	bool success = true;
	int hour = tm->tm_hour;

	success &= tm->tm_sec < 62;
	success &= tm->tm_min < 60;
	success &= hour < 24;

	if (success)
	{
		time.seconds = to_bcd(tm->tm_sec);
		time.minutes = to_bcd(tm->tm_min);
		if (ampm_mode)
		{
			time.hours |= AMPM_SELECT;
			if (hour > 11)
			{
				// Switch from 24-hour to 12-hour
				hour = hour - 12;
			}
			else
			{
				// Keep time as it is but set the AM bit
				hour |= AMPM_SET;
			}
			hour = hour > 11 ? hour - 12 : hour;
			time.hours |= to_bcd(hour);
		}
		else
		{
			time.hours = to_bcd(hour);
		}

		wr_fn(REG_SECS, (uint8_t*)&time, 3);
	}

	return success;
}

bool DS3231_SetDate(const TM * tm)
{
	DATE_REGISTERS date;
	bool success = true;

	int year = tm->tm_year;

	success &= tm->tm_mday < 31;
	success &= tm->tm_mday > 0;
	success &= tm->tm_mon < 12;
	success &= tm->tm_wday < 7;
	success &= tm->tm_yday < 366;

	if (success)
	{
		date.day = to_bcd(tm->tm_wday + 1);
		date.date = to_bcd(tm->tm_mday);
		date.month = to_bcd(tm->tm_mon);
		if (year > 99)
		{
			year -= 100;
			date.month = CENTURY_SELECT;
		}
		date.year = to_bcd(year);

		wr_fn(REG_DAY, (uint8_t*)&date, 4);
	}

	return success;
}

bool DS3231_SetDateTime(const TM * tm, bool ampm_mode)
{
	bool success = true;

	success &= DS3231_SetTime(tm, ampm_mode);
	success &= DS3231_SetDate(tm);

	return success;
}

void DS3231_GetTime(TM * tm)
{
	TIME_REGISTERS time;

	rd_fn(REG_SECS, (uint8_t*)&time, 3);

	tm->tm_sec = from_bcd(time.seconds);
	tm->tm_min = from_bcd(time.minutes);

	if (time.hours & AMPM_SELECT)
	{
		time.hours = time.hours > 11 ? time.hours - 12 : time.hours;
	}

	time.hours &= 0x3F;

	tm->tm_hour = from_bcd(time.hours);
}

void DS3231_GetDate(TM * tm)
{
	DATE_REGISTERS date;

	rd_fn(REG_DAY, (uint8_t*)&date, 4);

	tm->tm_wday = from_bcd(date.day);
	tm->tm_mday = from_bcd(date.date);

	tm->tm_mon = from_bcd(date.month & 0x1F);

	tm->tm_year = from_bcd(date.year);
	if (tm->tm_mon & CENTURY_SELECT)
	{
		tm->tm_year += 100;
	}

	tm->tm_yday = get_year_days(tm);


}

void DS3231_GetDateTime(TM * tm)
{
	DS3231_GetDate(tm);
	DS3231_GetTime(tm);
}

/*
 * Configure an alarm.
 * The time is assumed to be in 24-hour format.
 * The alarm interrupt must be configured seperately.
 */
bool DS3231_ConfigureAlarm(TM* tm, DS3231_ALARM_ENUM alarm, DS3231_DATE_TYPE_ENUM dateType, DS3231_TIME_FORMAT_ENUM timeFormat, DS3231_ALARM_RPT_ENUM repeat)
{
	bool success = true;

	// Test for valid ranges
	success &= (tm->tm_sec < 60);
	success &= (tm->tm_min < 60);

	switch (timeFormat)
	{
	case DS3231_TIME_FORMAT_24HR:
		success &= (tm->tm_hour < 24);
		break;
	case DS3231_TIME_FORMAT_AMPM:
		success &= (tm->tm_hour < 12);
		break;
	default:
		success = false;
		break;
	}

	switch (dateType)
	{
	case DS3232_DATE_TYPE_DAYOFMONTH:
		success &= (tm->tm_mday < days_in_month(tm->tm_mon));
		break;
	case DS3232_DATE_TYPE_DAYOFWEEK:
		success &= (tm->tm_mday < 7);
		break;
	default:
		success = false;
		break;
	}

	return success;
}

/*
 * Turn the internal TXCO oscillator on or off
 */
void DS3231_OscControl(bool on)
{
	if (on)
	{
		control &= ~EOSC;
	}
	else
	{
		control |= EOSC;
	}

	wr_fn(REG_CONTROL, &control, 1);
}

/*
 * Set the SWQ/INT pin to output either of square wave or alarm interrupt
 */
void DS3231_SQWINTControl(bool sqw_on, bool int_on)
{
	// Only one of SWQ and INT can be selected
	if (sqw_on && int_on) { return;}

	if (sqw_on)
	{
		control |= BBSWQ;
	}
	else
	{
		control &= ~BBSWQ;
	}

	if (int_on)
	{
		control |= INTCN;
	}
	else
	{
		control &= ~INTCN;
	}
	wr_fn(REG_CONTROL, &control, 1);
}

/*
 * Temperature conversions are normally every 64 seconds.
 * A conversion can be started manually if needed.
 * The BSY status should be checked for conversion complete
 */
void DS3231_StartTempConv(void)
{
	control |= CONV;
	wr_fn(REG_CONTROL, (uint8_t *)&control, 1);
	control &= ~CONV; // Conv bit is cleared when BSY is cleared (about 2ms)
}

/*
 * Set the square wave output rate
 */
void DS3231_SetRate(DS3231_RATE_ENUM rate)
{
	control &= ~(RS2 | RS1);
	control |= ((uint8_t)rate) << RS1_BIT;
	wr_fn(REG_CONTROL, &control, 1);
}

/*
 * Turn an alarm on/off
 */
void DS3231_AlarmControl(DS3231_ALARM_ENUM alarm, bool on)
{

	if (on)
	{
		control |= (DS3231_ALARM_1 == alarm) ? A1IE : A2IE;
	}
	else
	{
		control &= (DS3231_ALARM_1 == alarm) ? ~A1IE : ~A2IE;
	}
	wr_fn(REG_CONTROL, &control, 1);
}

/*
 * Read the status of the oscillator
 */
bool DS3231_ReadOscFlag(void)
{
	rd_fn(REG_STATUS, &status, 1);
	return (status & OSF) == OSF;
}

/*
 * If the oscillator flag has been set, must manually clear it
 */
void DS3231_ClearOscFlag(void)
{
	rd_fn(15, &status, 1);
	status &= ~OSF;
	wr_fn(15, &status, 1);
}

/*
 * Turn the 32kHz output on/off
 */
void DS3231_32KhzControl(bool on)
{
	rd_fn(REG_STATUS, &status, 1);

	if (on)
	{
		status |= EN32kHz;
	}
	else
	{
		status &= ~EN32kHz;
	}

	wr_fn(REG_STATUS, &status, 1);
}

/*
 * Read the status of the busy flag (for temperature conversions)
 */
bool DS3231_ReadBusyFlag(void)
{
	rd_fn(REG_STATUS, &status, 1);
	return (status & BSY) == BSY;
}

/*
 * Read the status of an alarm flag
 */
bool DS3231_ReadAlarmFlag(DS3231_ALARM_ENUM alarm)
{
	rd_fn(REG_STATUS, &status, 1);
	uint8_t mask = (DS3231_ALARM_1 == alarm) ? A1F : A2F;
	return (status & mask) == mask;
}

/*
 * Set the aging offset for the device
 */
void DS3231_SetAgingOffset(uint8_t offset)
{
	wr_fn(REG_AGING, &offset, 1); // Can just write straight to the register
}

/*
 * The temperature from the DS3231 is in 0.25 degree increments.
 * Return the temperature as an int16_t representing # of increments
 */
int16_t DS3231_ReadTemperatureQuarterDegrees(void)
{
	int16_t temperature;
	uint8_t temp[2];
	rd_fn(REG_TEMPMSB, temp, 2);

	/* Read the sign bit and clear it,
	as the shift operations will be easier
	without it */
	bool negative = temp[0] & 0x80;
	temp[0] &= 0x7F;

	temperature = temp[0] << 2;
	temperature += temp[1] >> 6;

	if (negative) {temperature = -temperature;}

	return temperature;
}

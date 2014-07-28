/*
 * Standard library includes
 */

#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <stddef.h>

/*
 * Utility Library Includes
 */

#include "util_macros.h"
#include "util_time.h"
#include "util_bcd.h"

/*
 * Device Library Includes
 */

#include "lib_ds3231.h"

/*
 * Generic Library Includes
 */

#ifndef UNITY_TEST
#include "lib_i2c_common.h"
#endif

/*
 * Private defines and typedefs
 */

#define DS3231_I2C_ADDRESS	0x68
#define	AMPM_SELECT		(1 << 6)
#define	PM_SET			(1 << 5)

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

#define REG_DATETIME_START	REG_SECS
#define REG_DATETIME_END	REG_YEAR
#define REG_DATETIME_LENGTH	(REG_DATETIME_END - REG_DATETIME_START + 1)

#define REG_TIME_START	REG_SECS
#define REG_TIME_END	REG_HRS
#define REG_TIME_LENGTH	(REG_TIME_END - REG_TIME_START + 1)

#define REG_DATE_START	REG_DAY
#define REG_DATE_END	REG_YEAR
#define REG_DATE_LENGTH	(REG_DATE_END - REG_DATE_START + 1)

#define	REG_ALRM1_SECS	7
#define	REG_ALRM1_MINS	8
#define	REG_ALRM1_HRS	9
#define	REG_ALRM1_DATE	10

#define REG_ALRM1_START		REG_ALRM1_SECS
#define REG_ALRM1_END		REG_ALRM1_DATE
#define REG_ALRM1_LENGTH	(REG_ALRM1_END - REG_ALRM1_START + 1)

#define	REG_ALRM2_MINS	11
#define	REG_ALRM2_HRS	12
#define	REG_ALRM2_DATE	13

#define REG_ALRM2_START		REG_ALRM2_MINS
#define REG_ALRM2_END		REG_ALRM2_DATE
#define REG_ALRM2_LENGTH	(REG_ALRM2_END - REG_ALRM2_START + 1)

#define	REG_CONTROL		14
#define	REG_STATUS		15
#define	REG_AGING		16
#define	REG_TEMPMSB		17
#define	REG_TEMPLSB		18

// Alarm hours register
#define ALRM_REG_SET_MASK_BIT	(7)
#define ALRM_REG_24_OR_12HR_BIT	(6)
#define ALRM_REG_AM_OR_PM_BIT	(5)

#define ALRM_REG_MASK_BIT		(1 << ALRM_REG_SET_MASK_BIT)
#define ALRM_REG_12_HR_BIT		(1 << ALRM_REG_24_OR_12HR_BIT)
#define ALRM_REG_24_HR_BIT		(0 << ALRM_REG_24_OR_12HR_BIT)

#define ALRM_REG_PM_BIT			(1 << ALRM_REG_AM_OR_PM_BIT)
#define ALRM_REG_AM_BIT			(0 << ALRM_REG_AM_OR_PM_BIT)

// Alarm day/date register
#define ALRM_REG_DYDT_BIT	(6)
#define ALRM_REG_DY_BIT		(1 << ALRM_REG_DYDT_BIT)
#define ALRM_REG_DT_BIT		(0 << ALRM_REG_DYDT_BIT)

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

struct datetime_registers
{
	TIME_REGISTERS time;
	DATE_REGISTERS date;
};
typedef struct datetime_registers DATETIME_REGISTERS;

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
	uint8_t s_control;
	uint8_t s_status;
	uint8_t aging;
	uint8_t temp_msb;
	uint8_t temp_lsb;
};
typedef struct control_registers CONTROL_REGISTERS;

/*
 * Private Variables
 */

static uint8_t s_control;
static uint8_t s_status;

static bool s_busy = false;

static DS3231_ONIDLE_FN s_onidle_cb;

static volatile DATETIME_REGISTERS s_dt = {
	.time = {
		.seconds = 0x59,
		.minutes = 0x59,
		.hours = 0x23,
		},
	.date = {
		.day = 0x03,
		.date = 0x22,
		.month = 0x09,
		.year = 0x10
		}
};

static uint8_t s_temperature[2];

#ifndef UNITY_TEST
static I2C_TRANSFER_DATA s_i2c_data;
static I2C_TRANSFER_DATA s_i2c_setup;

static uint8_t s_WriteArray[19];
#endif

/*
 * Private Function Prototypes
 */
 
static void write(uint8_t reg, uint8_t* array, uint8_t n, DS3231_ONIDLE_FN cb);
static void read(uint8_t reg, uint8_t* array, uint8_t n, DS3231_ONIDLE_FN cb);

#ifndef UNITY_TEST
static void rd_callback(I2C_TRANSFER_DATA * transfer);
static void wr_callback(I2C_TRANSFER_DATA * transfer);
#endif

static bool setLocalDate(const TM * tm);
static bool setLocalTime(const TM * tm, bool ampm_mode);

static bool SetAlarm1Mask(ALARM_REGISTERS * alarm, DS3231_ALARM_RPT_ENUM repeat);
static bool SetAlarm2Mask(ALARM_REGISTERS * alarm, DS3231_ALARM_RPT_ENUM repeat);

/*
 * Public Functions
 */

bool DS3231_Init(void)
{
	#ifdef UNITY_TEST
	return true;
	#else
	return I2C_Init(NULL);
	#endif
}

bool DS3231_SetDeviceTime(const TM * tm, bool ampm_mode, DS3231_ONIDLE_FN cb)
{
	bool success = true;
	
	if (!s_busy)
	{
		if ( setLocalTime(tm, ampm_mode) )
		{
			write(REG_TIME_START, (uint8_t*)&s_dt, REG_TIME_LENGTH, cb);
		}
	}
	return success;
}

bool DS3231_SetDeviceDate(const TM * tm, DS3231_ONIDLE_FN cb)
{
	bool success = true;
	if (!s_busy)
	{
		if ( setLocalDate(tm) )
		{
			write(REG_DATE_START, (uint8_t*)&s_dt, REG_DATE_LENGTH, cb);
		}
	}
	
	return success;
}

bool DS3231_SetDeviceDateTime(const TM * tm, bool ampm_mode, DS3231_ONIDLE_FN cb)
{
	bool success = true;

	if (!s_busy)
	{
		success &= setLocalTime(tm, ampm_mode);
		success &= setLocalDate(tm);
		write(REG_DATETIME_START, (uint8_t*)&s_dt, REG_DATETIME_LENGTH, cb);
	}

	return success;
}

void DS3231_ReadDeviceDateTime(DS3231_ONIDLE_FN cb)
{
	if (!s_busy)
	{
		read(REG_DATETIME_START, (uint8_t*)&s_dt, REG_DATETIME_LENGTH, cb);
	}
}

void DS3231_GetTime(TM * tm)
{
	if (!s_busy)
	{
		tm->tm_sec = from_bcd(s_dt.time.seconds);
		tm->tm_min = from_bcd(s_dt.time.minutes);

		bool time_in_12hr_format = (bool)(s_dt.time.hours & AMPM_SELECT);
		bool pm_bit_set = (bool)(s_dt.time.hours & PM_SET);

		if (time_in_12hr_format)
		{
			s_dt.time.hours &= 0x1F;
			tm->tm_hour = from_bcd(s_dt.time.hours);
			if (pm_bit_set)
			{
				tm->tm_hour = min(tm->tm_hour + 12, 23); //Convert from 1pm-12pm to 1300 to 2300
			}
		}
		else
		{
			s_dt.time.hours &= 0x3F;
			tm->tm_hour = from_bcd(s_dt.time.hours);
		}
	}
}

void DS3231_GetDate(TM * tm)
{
	if (!s_busy)
	{
		tm->tm_wday = from_bcd(s_dt.date.day) - 1;
		tm->tm_mday = from_bcd(s_dt.date.date);

		tm->tm_mon = from_bcd(s_dt.date.month & 0x1F) - 1;

		tm->tm_year = from_bcd(s_dt.date.year);
		if (s_dt.date.month & CENTURY_SELECT)
		{
			tm->tm_year += 100;
		}

		tm->tm_yday = calculate_days_into_year(tm);
	}
}

void DS3231_GetDateTime(TM * tm)
{
	if (!s_busy)
	{
		DS3231_GetDate(tm);
		DS3231_GetTime(tm);
	}
}

uint8_t DS3231_GetRegisterValue(uint8_t regIndex)
{
	return ((uint8_t *)(&s_dt))[regIndex];
}

/*
 * Configure an alarm.
 * The time is assumed to be in 24-hour format.
 * The alarm interrupt must be configured seperately.
 */
bool DS3231_ConfigureAlarm(TM* tm, DS3231_ALARM_ENUM eAlarm, DS3231_DATE_TYPE_ENUM dateType, DS3231_TIME_FORMAT_ENUM timeFormat, bool pm, DS3231_ALARM_RPT_ENUM repeat)
{
	bool success = true;
	ALARM_REGISTERS alarm;
	
	alarm.seconds = 0;
	alarm.minutes = 0;
	alarm.hours = 0;
	alarm.day_date = 0;
	
	// Test for valid ranges
	success &= (tm->tm_sec < 60);
	success &= (tm->tm_min < 60);

	if (success)
	{
		alarm.seconds = to_bcd(tm->tm_sec);
		alarm.minutes = to_bcd(tm->tm_min);
	}
	
	/* Set 24 or 12 hour time */
	switch (timeFormat)
	{
	case DS3231_TIME_FORMAT_24HR:
		success &= (tm->tm_hour < 24);
		if (success)
		{
			alarm.hours = to_bcd(tm->tm_hour);
			alarm.hours |= ALRM_REG_24_HR_BIT;
		}
		break;
	case DS3231_TIME_FORMAT_AMPM:
		success &= (tm->tm_hour < 12);
		if (success)
		{
			alarm.hours = to_bcd(tm->tm_hour);
			alarm.hours |= ALRM_REG_12_HR_BIT;
			alarm.hours |= (pm ? ALRM_REG_PM_BIT : ALRM_REG_AM_BIT);
		}
		break;
	default:
		success = false;
		break;
	}

	/* Set the date or day-of-week */
	switch (dateType)
	{
	case DS3232_DATE_TYPE_DAYOFMONTH:
		success &= (tm->tm_mday < (int)days_in_month(tm->tm_mon, is_leap_year(C_TO_GREGORIAN_YEAR(tm->tm_year))));
		if (success)
		{
			alarm.day_date = to_bcd(tm->tm_mday);
			alarm.day_date |= ALRM_REG_DT_BIT;
		}
		break;
	case DS3232_DATE_TYPE_DAYOFWEEK:
		success &= (tm->tm_wday < 7);
		if (success)
		{
			alarm.day_date = to_bcd(tm->tm_wday + 1);
			alarm.day_date |= ALRM_REG_DY_BIT;
		}
		break;
	default:
		success = false;
		break;
	}
	
	/* Selectable alarm repeat */
	if (success)
	{
		switch(eAlarm)
		{
		case DS3231_ALARM_1:
			success &= SetAlarm1Mask(&alarm, repeat);
			break;
		case DS3231_ALARM_2:
			success &= SetAlarm2Mask(&alarm, repeat);
			break;
		}
	}
	
	if (success)
	{
		switch(eAlarm)
		{
		case DS3231_ALARM_1:
			write(REG_ALRM1_START, (uint8_t*)(&(alarm.seconds)), REG_ALRM1_LENGTH, NULL);
			break;
		case DS3231_ALARM_2:
			write(REG_ALRM2_START, (uint8_t*)(&(alarm.minutes)), REG_ALRM2_LENGTH, NULL);
			break;
		}
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
		s_control &= ~EOSC;
	}
	else
	{
		s_control |= EOSC;
	}
}

void DS3231_BBSWQ_Control(bool on)
{
	if (on)
	{
		s_control |= BBSWQ;
	}
	else
	{
		s_control &= ~BBSWQ;
	}
}

/*
 * Set the SWQ/INT pin to output either of square wave or alarm interrupt
 */
void DS3231_SQWINTControl(DS3231_SQWINT_ENUM selection)
{
	switch(selection)
	{
	case DS3231_SQW:
		s_control &= ~INTCN;
		break;
	case DS3231_INT:
		s_control |= INTCN;
		break;
	}
}

void DS3231_UpdateControl(void)
{
	write(REG_CONTROL, &s_control, 1, NULL);
}
/*
 * Temperature conversions are normally every 64 seconds.
 * A conversion can be started manually if needed.
 * The BSY s_status should be checked for conversion complete
 */
void DS3231_StartTempConv(void)
{
	s_control |= CONV;
	write(REG_CONTROL, &s_control, 1, NULL);
	// The conv bit on DS3231 is cleared when BSY is cleared (about 2ms)
	// TODO: Actually read the BSY and CONV bits before trying a CONV
	s_control &= ~CONV;
}

/*
 * Set the square wave output rate
 */
void DS3231_SetRate(DS3231_RATE_ENUM rate)
{
	s_control &= ~(RS2 | RS1);
	s_control |= ((uint8_t)rate) << RS1_BIT;
}

/*
 * Turn an alarm on/off
 */
void DS3231_AlarmControl(DS3231_ALARM_ENUM alarm, bool on)
{
	if (on)
	{
		s_control |= (DS3231_ALARM_1 == alarm) ? A1IE : A2IE;
	}
	else
	{
		s_control &= (DS3231_ALARM_1 == alarm) ? ~A1IE : ~A2IE;
	}
}

/*
 * Update the local shadow of the s_status register
 */
void DS3231_ReadStatus(DS3231_ONIDLE_FN cb)
{
	if(!s_busy)
	{
		read(REG_STATUS, &s_status, 1, cb);
	}
}

/*
 * Read the s_status of the oscillator
 */
bool DS3231_GetOscFlag(void)
{
	return (s_status & OSF) == OSF;
}

/*
 * If the oscillator flag has been set, must manually clear it
 * NOTE: this function does not read the s_status before clearing!
 */
void DS3231_ClearOscFlag(void)
{
	if (!s_busy)
	{
		s_status &= ~OSF;
		write(REG_STATUS, &s_status, 1, NULL);
	}
}

/*
 * Turn the 32kHz output on/off
 */
void DS3231_32KhzControl(bool on)
{
	if (!s_busy)
	{
		if (on)
		{
			s_status |= EN32kHz;
		}
		else
		{
			s_status &= ~EN32kHz;
		}

		write(REG_STATUS, &s_status, 1, NULL);
	}
}

/*
 * Read the s_status of the s_busy flag (for s_temperature conversions)
 */
bool DS3231_GetBusyFlag(void)
{
	return (s_status & BSY) == BSY;
}

/*
 * Read the s_status of an alarm flag
 */
bool DS3231_GetAlarmFlag(DS3231_ALARM_ENUM alarm)
{
	uint8_t mask = (DS3231_ALARM_1 == alarm) ? A1F : A2F;
	return (s_status & mask) == mask;
}

/*
 * Set the aging offset for the device
 */
void DS3231_SetAgingOffset(uint8_t offset)
{
	if (!s_busy)
	{
		write(REG_AGING, &offset, 1, NULL); // Can just write straight to the register
	}
}

/*
 * The s_temperature from the DS3231 is in 0.25 degree increments.
 * Return the s_temperature as an int16_t representing # of increments
 */
void DS3231_UpdateTemperature(DS3231_ONIDLE_FN cb)
{
	if (!s_busy)
	{
		read(REG_TEMPMSB, s_temperature, 2, cb);
	}
}

int16_t DS3231_GetTemperatureQuarterDegrees(void)
{
	int16_t return_temperature;
	
	/* Read the sign bit and clear it,
	as the shift operations will be easier
	without it */
	bool negative = s_temperature[0] & 0x80;
	s_temperature[0] &= 0x7F;

	return_temperature = s_temperature[0] << 2;
	return_temperature += s_temperature[1] >> 6;

	if (negative) {return_temperature = -return_temperature;}

	return return_temperature;
}

bool DS3231_IsIdle(void)
{
	return !s_busy;
}

/*
 * Private Functions
 */

static bool setLocalDate(const TM * tm)
{
	bool success = true;
	int year = tm->tm_year;

	success &= tm->tm_mday < 32;
	success &= tm->tm_mday > 0;
	success &= tm->tm_mon < 12;
	success &= tm->tm_wday < 7;
	success &= tm->tm_yday < 366;

	if (success)
	{
		s_dt.date.day = to_bcd(tm->tm_wday + 1);
		s_dt.date.date = to_bcd(tm->tm_mday);
		s_dt.date.month = to_bcd(tm->tm_mon + 1);
		
		while (year > 99)
		{
			year -= 100;
			s_dt.date.month ^= CENTURY_SELECT;
		}
		s_dt.date.year = to_bcd(year);
	}
	
	return success;
}

static bool setLocalTime(const TM * tm, bool ampm_mode)
{
	bool success = true;
	
	int hour = tm->tm_hour;

	success &= tm->tm_sec < 62;
	success &= tm->tm_min < 60;
	success &= hour < 24;

	if (success)
	{
		s_dt.time.seconds = to_bcd(tm->tm_sec);
		s_dt.time.minutes = to_bcd(tm->tm_min);
		if (ampm_mode)
		{
			s_dt.time.hours = AMPM_SELECT;
			if (hour == 0)
			{
				hour = 12;
				// Leave PM bit cleared
			}
			else if (hour > 12)
			{
				// Switch from 24-hour to 12-hour
				hour = hour - 12;
				s_dt.time.hours |= PM_SET;
			}
			s_dt.time.hours |= to_bcd(hour);
		}
		else
		{
			s_dt.time.hours = to_bcd(hour);
		}
	}
	
	return success;
}
static bool SetAlarm1Mask(ALARM_REGISTERS * alarm, DS3231_ALARM_RPT_ENUM repeat)
{

	bool success = true;
	
	switch(repeat)
	{
	case DS3231_ALARM_RPT_EVERY_S:
		alarm->seconds |= ALRM_REG_MASK_BIT;
		alarm->minutes |= ALRM_REG_MASK_BIT;
		alarm->hours |= ALRM_REG_MASK_BIT;
		alarm->day_date |= ALRM_REG_MASK_BIT;
		break;
	case DS3231_ALARM_RPT_MATCH_S:
		alarm->minutes |= ALRM_REG_MASK_BIT;
		alarm->hours |= ALRM_REG_MASK_BIT;
		alarm->day_date |= ALRM_REG_MASK_BIT;
		break;
	case DS3231_ALARM_RPT_MATCH_MS:
		alarm->hours |= ALRM_REG_MASK_BIT;
		alarm->day_date |= ALRM_REG_MASK_BIT;
		break;
	case DS3231_ALARM_RPT_MATCH_HMS:
		alarm->day_date |= ALRM_REG_MASK_BIT;
		break;
	case DS3231_ALARM_RPT_MATCH_DHMS:
		break;
	default:
		success = false;
		break;
	}
	
	return success;
}

static bool SetAlarm2Mask(ALARM_REGISTERS * alarm, DS3231_ALARM_RPT_ENUM repeat)
{

	bool success = true;
	
	switch(repeat)
	{
	case DS3231_ALARM_RPT_EVERY_M:
		alarm->minutes |= ALRM_REG_MASK_BIT;
		alarm->hours |= ALRM_REG_MASK_BIT;
		alarm->day_date |= ALRM_REG_MASK_BIT;
		break;
	case DS3231_ALARM_RPT_MATCH_M:
		alarm->hours |= ALRM_REG_MASK_BIT;
		alarm->day_date |= ALRM_REG_MASK_BIT;
		break;
	case DS3231_ALARM_RPT_MATCH_HM:
		alarm->day_date |= ALRM_REG_MASK_BIT;
		break;
	case DS3231_ALARM_RPT_MATCH_DHMS:
		break;
	default:
		success = false;
		break;
	}
	
	return success;
}

static void write(uint8_t reg, uint8_t* array, uint8_t n, DS3231_ONIDLE_FN cb)
{
	#ifndef UNITY_TEST
	s_busy = true;
	s_WriteArray[0] = reg;
	memcpy(&s_WriteArray[1], array, n);
	s_i2c_data.buffer = s_WriteArray;
	s_i2c_data.totalBytes = n+1;
	s_i2c_data.address = DS3231_I2C_ADDRESS;
	s_i2c_data.callback = wr_callback;

	s_onidle_cb = cb;
	I2C_StartMaster(&s_i2c_data, false, false);
	#else
	(void)reg;
	(void)array;
	(void)n;
	(void)cb;
	#endif

}

static void read(uint8_t reg, uint8_t* array, uint8_t n, DS3231_ONIDLE_FN cb)
{
	#ifndef UNITY_TEST
	s_busy = true;

	// Save the read transfer data into s_i2c_data for later.
	s_i2c_data.buffer = array;
	s_i2c_data.totalBytes = n;
	s_i2c_data.address = DS3231_I2C_ADDRESS;
	s_i2c_data.callback = rd_callback;
	s_onidle_cb = cb;

	// First there needs to be a write to the register pointer
	s_WriteArray[0] = reg;
	s_i2c_setup.buffer = s_WriteArray;
	s_i2c_setup.totalBytes = 1;
	s_i2c_setup.address = DS3231_I2C_ADDRESS;
	s_i2c_setup.callback = rd_callback;

	// And request a write with repeated start
	I2C_StartMaster(&s_i2c_setup, false, true);
	#else
	(void)reg;
	(void)array;
	(void)n;
	(void)cb;
	#endif
}

#ifndef UNITY_TEST
static void rd_callback(I2C_TRANSFER_DATA * transfer)
{
	if (transfer == &s_i2c_setup)
	{
		// The read register has been set, can now start the read
		I2C_StartMasterFromRS(&s_i2c_data, true, false);
	}
	else
	{
		s_busy = false;
		if (s_onidle_cb)
		{
			s_onidle_cb(false);
		}
	}
}

static void wr_callback(I2C_TRANSFER_DATA * transfer)
{
	(void)transfer;
	s_busy = false;
	if (s_onidle_cb)
	{
		s_onidle_cb(true);
	}
}
#endif

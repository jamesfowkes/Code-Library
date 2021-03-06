#ifndef _LIB_DS3231_H_
#define _LIB_DS3231_H_

/*
 * Defines and typedefs
 */

typedef void (*DS3231_ONIDLE_FN)(bool write);

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

enum ds3231_rate_enum
{
	DS3231_RATE_1HZ,
	DS3231_RATE_1024HZ,
	DS3231_RATE_4096HZ,
	DS3231_RATE_8192HZ,
};
typedef enum ds3231_rate_enum DS3231_RATE_ENUM;

enum ds3231_alarm_rpt_enum
{
	DS3231_ALARM_RPT_EVERY_S,
	DS3231_ALARM_RPT_MATCH_S,
	DS3231_ALARM_RPT_EVERY_M,
	DS3231_ALARM_RPT_MATCH_M,
	DS3231_ALARM_RPT_MATCH_MS,
	DS3231_ALARM_RPT_MATCH_HM,
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

enum ds3231_sqwint_enum
{
	DS3231_SQW,
	DS3231_INT
};
typedef enum ds3231_sqwint_enum DS3231_SQWINT_ENUM;

/*
 * Public Function Prototypes
 */

bool DS3231_Init(void);

bool DS3231_SetDeviceTime(const TM *, bool ampm_mode, DS3231_ONIDLE_FN cb);
bool DS3231_SetDeviceDate(const TM *, DS3231_ONIDLE_FN cb);
bool DS3231_SetDeviceDateTime(const TM * tm, bool ampm_mode, DS3231_ONIDLE_FN cb);

void DS3231_ReadDeviceDateTime(DS3231_ONIDLE_FN cb);
void DS3231_GetDate(TM * tm);
void DS3231_GetTime(TM * tm);
void DS3231_GetDateTime(TM * tm);

void DS3231_AlarmControl(DS3231_ALARM_ENUM alarm, bool on);
bool DS3231_ConfigureAlarm(TM* tm, DS3231_ALARM_ENUM alarm, DS3231_DATE_TYPE_ENUM dateType, DS3231_TIME_FORMAT_ENUM timeFormat, bool pm, DS3231_ALARM_RPT_ENUM repeat);

void DS3231_UpdateControl(void);

void DS3231_OscControl(bool on);

void DS3231_SetRate(DS3231_RATE_ENUM rate);
void DS3231_SQWINTControl(DS3231_SQWINT_ENUM selection);

void DS3231_StartTempConv(void);
void DS3231_UpdateTemperature(DS3231_ONIDLE_FN cb);
int16_t DS3231_GetTemperatureQuarterDegrees(void);

void DS3231_32KhzControl(bool on);

void DS3231_ReadStatus(DS3231_ONIDLE_FN cb);
void DS3231_ClearOscFlag(void);

bool DS3231_GetOscFlag(void);
bool DS3231_GetBusyFlag(void);
bool DS3231_GetAlarmFlag(DS3231_ALARM_ENUM alarm);

void DS3231_SetAgingOffset(uint8_t offset);

bool DS3231_IsIdle(void);

uint8_t DS3231_GetRegisterValue(uint8_t regIndex);

#endif

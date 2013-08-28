#ifndef _LIB_DS3231_H_
#define _LIB_DS3231_H_

/*
 * Defines and typedefs
 */

typedef void (*I2C_WR_FN)(uint8_t reg, uint8_t* array, uint8_t n, I2C_CALLBACK cb);
typedef void (*I2C_RD_FN)(uint8_t reg, uint8_t* array, uint8_t n, I2C_CALLBACK cb);

typedef void (*DS3231_ONIDLE_FN)(bool write);

bool DS3231_Init(I2C_WR_FN _wr_fn, I2C_WR_FN _rd_fn);

bool DS3231_SetTime(const TM *, bool ampm_mode, DS3231_ONIDLE_FN cb);
bool DS3231_SetDate(const TM *, DS3231_ONIDLE_FN cb);
bool DS3231_SetDateTime(const TM * tm, bool ampm_mode, DS3231_ONIDLE_FN cb);

void DS3231_ReadDateTime(DS3231_ONIDLE_FN cb);
void DS3231_GetDate(TM * tm);
void DS3231_GetTime(TM * tm);
void DS3231_GetDateTime(TM * tm);

void DS3231_AlarmControl(DS3231_ALARM_ENUM alarm, bool on);
bool DS3231_ConfigureAlarm(TM* tm, DS3231_ALARM_ENUM alarm, DS3231_DATE_TYPE_ENUM dateType, DS3231_TIME_FORMAT_ENUM timeFormat, DS3231_ALARM_RPT_ENUM repeat);

void DS3231_OscControl(bool on);

void DS3231_SetRate(DS3231_RATE_ENUM rate);
void DS3231_SQWINTControl(bool sqw_on, bool int_on);

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

#endif

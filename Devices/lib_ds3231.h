#ifndef _LIB_DS3231_H_
#define _LIB_DS3231_H_

typedef void (*I2C_WR_FN)(uint8_t reg, uint8_t* array, uint8_t n);
typedef void (*I2C_RD_FN)(uint8_t reg, uint8_t* array, uint8_t n);

bool DS3231_Init(I2C_WR_FN _wr_fn, I2C_WR_FN _rd_fn);

bool DS3231_SetTime(const TM *, bool ampm_mode);
bool DS3231_SetDate(const TM *);
bool DS3231_SetDateTime(const TM * tm, bool ampm_mode);

void DS3231_GetDate(TM *);
void DS3231_GetTime(TM *);
void DS3231_GetDateTime(TM * tm);

void DS3231_AlarmControl(DS3231_ALARM_ENUM alarm, bool on);
bool DS3231_ConfigureAlarm(TM* tm, DS3231_ALARM_ENUM alarm, DS3231_DATE_TYPE_ENUM dateType, DS3231_TIME_FORMAT_ENUM timeFormat, DS3231_ALARM_RPT_ENUM repeat);

void DS3231_OscControl(bool on);

bool DS3231_ReadOscFlag(void);
void DS3231_ClearOscFlag(void);

void DS3231_SetRate(DS3231_RATE_ENUM rate);
void DS3231_SQWINTControl(bool sqw_on, bool int_on);

void DS3231_StartTempConv(void);
int16_t DS3231_ReadTemperatureQuarterDegrees(void);

void DS3231_32KhzControl(bool on);

bool DS3231_ReadBusyFlag(void);

bool DS3231_ReadAlarmFlag(DS3231_ALARM_ENUM alarm);

void DS3231_SetAgingOffset(uint8_t offset);

#endif

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#include "util_time.h"
#include "util_bcd.h"

#include "lib_DS3231.h"

#define AMPM_BIT 5
#define TWENTY_HR_BIT 5

#define HRS_MODE_BIT 6

void testMinutesAndSeconds(void);
void test12Hours(void);
void test24Hours(void);
void testDayDate(void);
void testMonths(void);
void testYears(void);

void testDS3231(void);

void printTM(TM * tm);

void parseSecondsRegister(uint8_t reg, uint8_t expected);
void parseMinutesRegister(uint8_t reg, uint8_t expected);
void parseHoursRegister(uint8_t reg, uint8_t expected, bool expectedHoursBit, bool expectedAMPM);
void parseDayRegister(uint8_t reg, uint8_t expected);
void parseDateRegister(uint8_t reg, uint8_t expected);
void parseMonthRegister(uint8_t reg, uint8_t expected, bool centuryBit);
void parseYearsRegister(uint8_t reg, uint8_t expected);

static uint32_t s = 0;

int main(void)
{
	testMinutesAndSeconds();
	test12Hours();
	test24Hours();
	testDayDate();
	testMonths();
	testYears();
	testDS3231();
	return 0;
}

void testMinutesAndSeconds(void)
{
	printf("Testing minutes/seconds registers\n");
	uint8_t value;
	uint8_t bcd;

	for (value = 0; value < 59; value++)
	{
		bcd = to_bcd(value);
		parseSecondsRegister(bcd, value);
	}
	
	for (value = 0; value < 59; value++)
	{
		bcd = to_bcd(value);
		parseMinutesRegister(bcd, value);
	}
}

void test12Hours(void)
{
	printf("Testing hours register (12 hour mode)\n");
	uint8_t hour = 0;
	uint8_t bcd;
	bool am = true;
	
	for (hour = 0; hour < 13; hour++)
	{
		am = true;
		bcd = to_bcd(hour);
		bcd &= ~(1 << AMPM_BIT);
		bcd |= (1 << HRS_MODE_BIT);
		parseHoursRegister(bcd, hour, true, !am);
	}
	
	for (hour = 1; hour < 11; hour++)
	{
		am = false;
		bcd = to_bcd(hour);
		bcd |= (1 << AMPM_BIT);
		bcd |= (1 << HRS_MODE_BIT);
		parseHoursRegister(bcd, hour, true, !am);
	}
}

void test24Hours(void)
{
	printf("Testing hours register (24 hour mode)\n");
	uint8_t hour = 0;
	uint8_t bcd;
	
	for (hour = 0; hour < 24; hour++)
	{
		bcd = to_bcd(hour);
		parseHoursRegister(bcd, hour, false, (hour >= 20));
	}
}

void testDayDate(void)
{
	printf("Testing day and date registers\n");
	uint8_t day;
	uint8_t date;
	uint8_t bcd;
	
	for (day = 1; day < 8; day++)
	{
		bcd = to_bcd(day);
		parseDayRegister(bcd, day);
	}
	
	for (date = 1; date < 32; date++)
	{
		bcd = to_bcd(date);
		parseDateRegister(bcd, date );
	}
}

void testMonths(void)
{
	printf("Testing months register\n");
	uint8_t month;
	uint8_t bcd;

	for (month = 1; month < 13; month++)
	{
		bcd = to_bcd(month);
		parseMonthRegister(bcd, month, false);
	}
}

void testYears(void)
{
	printf("Testing years register\n");
	uint8_t year;
	uint8_t bcd;

	for (year = 0; year < 100; year++)
	{
		bcd = to_bcd(year);
		parseYearsRegister(bcd, year);
	}
}

void testDS3231(void)
{
	TM tm;
	
	printf("Testing DS3231\n");
	
	int year = 0;
	
	s = 946684800; // 30/12/1999 00:00:00
	uint32_t end = 0xFFFFFFFF;
	
	while (s++ < end)
	{
		unix_seconds_to_time(s, &tm);
		
		DS3231_SetDateTime(&tm, false, NULL);
		
		parseSecondsRegister(DS3231_GetRegisterValue(0), tm.tm_sec);
		parseMinutesRegister(DS3231_GetRegisterValue(1), tm.tm_min);
		parseHoursRegister(DS3231_GetRegisterValue(2), tm.tm_hour, false, (tm.tm_hour >= 20));
		parseDayRegister(DS3231_GetRegisterValue(3), tm.tm_wday + 1); // Days 0-6 map to 1-7
		parseDateRegister(DS3231_GetRegisterValue(4), tm.tm_mday);
		parseMonthRegister(DS3231_GetRegisterValue(5), tm.tm_mon + 1, (tm.tm_year > 99)); // Months 0-11 map to 1-12
		parseYearsRegister(DS3231_GetRegisterValue(6), tm.tm_year % 100);
		
		if (year != tm.tm_year)
		{
			year = tm.tm_year;
			printf("Year = %d\n", 1900+tm.tm_year);
		}
	}
}

void printTM(TM * tm)
{
	int * pTM = (int*)tm;
	
	uint32_t i = 0;
	for (i = 0; i < 8; ++i)
	{
		printf("%d, ", pTM[i]);
	}
}

void parseSecondsRegister(uint8_t reg, uint8_t expected)
{
	uint8_t seconds = (reg & 0x0F);
	seconds += (((reg & 0xF0) >> 4) * 10);
	if (reg & 0x80) { printf("Non-zero leading bit"); }
	if (seconds != expected)
	{
		printf("0x%02x = %ds, expected %d\n", reg, seconds, expected);
	}
}

void parseMinutesRegister(uint8_t reg, uint8_t expected)
{
	uint8_t minutes = (reg & 0x0F);
	minutes += (((reg & 0xF0) >> 4) * 10);
	if (reg & 0x80) { printf("Non-zero leading bit"); }
	if (minutes != expected)
	{
		printf("0x%02x = %dm\n", reg, minutes);
	}
}

void parseHoursRegister(uint8_t reg, uint8_t expected, bool expectedHoursBit, bool expectedAM)
{
	uint8_t hours = 0;
	if (expectedHoursBit)
	{
		hours = (reg & 0x0F);
		hours += (((reg & 0x10) >> 4) * 10);
	}
	else
	{
		hours = (reg & 0x0F);
		hours += (((reg & 0x30) >> 4) * 10);
	}
	
	bool am = (reg & (1 << AMPM_BIT));
	bool hour_mode = (reg & (1 << HRS_MODE_BIT));
	if (hours != expected)
	{
		printf("0x%02x = %dh, expected %d\n", reg, hours, expected);
	}
	
	if (am != expectedAM)
	{
		printf("Unexpected AMPM bit in 0x%02x\n", reg);
	}
	
	if (hour_mode != expectedHoursBit)
	{
		printf("Unexpected hours bit in 0x%02x\n", reg);
	}
}

void parseDayRegister(uint8_t reg, uint8_t expected)
{
	char * days[] = {"???", "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
	uint8_t day = (reg & 0x07);
	if (reg & 0xF8) { printf("Non-zero leading bits"); }
	if (day != expected)
	{
		printf("0x%02x = %s, expected %s (%lu)\n", reg, days[day], days[expected], s);
	}
}

void parseDateRegister(uint8_t reg, uint8_t expected)
{
	uint8_t date = (reg & 0x0F);
	date += (((reg & 0xF0) >> 4) * 10);
	if (reg & 0xC0) { printf("Non-zero leading bits"); }
	if (date != expected)
	{
		printf("Date: 0x%02x = %d, expected %d\n", reg, date, expected);
	}
}

void parseMonthRegister(uint8_t reg, uint8_t expected, bool centuryBit)
{
	char * months[] = {"???", "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
	uint8_t month = (reg & 0x0F);
	month += (((reg & 0x10) >> 4) * 10);
	if (reg & 0x60) { printf("Non-zero leading bits"); }
	if (month != expected)
	{
		printf("0x%02x = %s expected %s (%lu)\n", reg, months[month], months[expected], s);
	}
	
	if ((bool)(reg & 0x80) ^ (centuryBit))
	{
		printf("Incorrect century bit. Expected %s, was %s\n", centuryBit ? "on" : "off", (reg & 0x80) ? "on" : "off");
	}
}

void parseYearsRegister(uint8_t reg, uint8_t expected)
{
	uint8_t year = (reg & 0x0F);
	year += (((reg & 0xF0) >> 4) * 10);
	if (year != expected)
	{
		printf("Year 0x%02x = %d, expected %d \n", reg, year, expected);
	}
}

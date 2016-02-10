#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#include <iostream>

#include "Utility/util_time.h"
#include "Utility/util_bcd.h"

#include "lib_ds3231.h"

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

void testWR(void);

void printTM(TM * tm);

void parseSecondsRegister(uint8_t reg, uint8_t expected);
void parseMinutesRegister(uint8_t reg, uint8_t expected);
void parseHoursRegister(uint8_t reg, uint8_t expected, bool expectedHoursBit, bool expectedAMPM);
void parseDayRegister(uint8_t reg, uint8_t expected);
void parseDateRegister(uint8_t reg, uint8_t expected);
void parseMonthRegister(uint8_t reg, uint8_t expected, bool centuryBit);
bool parseYearsRegister(uint8_t reg, uint8_t expected);

static uint32_t s = 0;

void setUp(void) {};
void tearDown(void) {};

void test_All(void)
{
	testMinutesAndSeconds();
	test12Hours();
	test24Hours();
	testDayDate();
	testMonths();
	testYears();
	testDS3231();
	testWR();
}

void testMinutesAndSeconds(void)
{
	std::cout << "Testing minutes/seconds registers" << std::endl;
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
	std::cout << "Testing hours register (12 hour mode)" << std::endl;
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
	std::cout << "Testing hours register (24 hour mode)" << std::endl;
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
	std::cout << "Testing day and date registers" << std::endl;
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
	std::cout << "Testing months register" << std::endl;
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
	std::cout << "Testing years register" << std::endl;
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
	
	std::cout << "Testing DS3231" << std::endl;
	
	int year = 0;
	
	s = 7819200UL;
	uint32_t end = (131UL*365UL*24UL*60UL*60UL);
	
	while (s < end)
	{
		s = s + (S_PER_DAY * 365);
		unix_seconds_to_time(s, &tm);
		
		DS3231_SetDeviceDateTime(&tm, false, NULL);
		
		parseSecondsRegister(DS3231_GetRegisterValue(0), tm.tm_sec);
		parseMinutesRegister(DS3231_GetRegisterValue(1), tm.tm_min);
		parseHoursRegister(DS3231_GetRegisterValue(2), tm.tm_hour, false, (tm.tm_hour >= 20));
		parseDayRegister(DS3231_GetRegisterValue(3), tm.tm_wday + 1); // Days 0-6 map to 1-7
		parseDateRegister(DS3231_GetRegisterValue(4), tm.tm_mday);
		parseMonthRegister(DS3231_GetRegisterValue(5), tm.tm_mon + 1, (tm.tm_year / 100) % 2); // Months 0-11 map to 1-12
		if ( !parseYearsRegister(DS3231_GetRegisterValue(6), tm.tm_year % 100) )
		{
			std::cout << s << std::endl;
		}
		
		if (year != tm.tm_year)
		{
			year = tm.tm_year;
		}
	}
}

void testWR(void)
{
	TM tm_in;
	TM tm_out;
	std::cout << "Testing write then read" << std::endl;
	uint32_t in = 1382554522;
	uint32_t out = 0;
	unix_seconds_to_time(in, &tm_in);
	
	printTM(&tm_in);	

	DS3231_SetDeviceDateTime(&tm_in, false, NULL);

	std::cout << "Hours: " << (int)DS3231_GetRegisterValue(2);

	DS3231_GetDateTime(&tm_out);

	printTM(&tm_out);

	out = time_to_unix_seconds(&tm_out);

	std::cout << "In = " << in << " out = " << out << std::endl;
}

void printTM(TM * tm)
{
	int * pTM = (int*)tm;
	
	uint32_t i = 0;
	for (i = 0; i < 8; ++i)
	{
		std::cout << pTM[i];
		if (i < 7) {std::cout << ", ";}
	}
	std::cout << std::endl;
}

void parseSecondsRegister(uint8_t reg, uint8_t expected)
{
	uint8_t seconds = (reg & 0x0F);
	seconds += (((reg & 0xF0) >> 4) * 10);
	if (reg & 0x80) { std::cout << "Non-zero leading bit"; }
	if (seconds != expected)
	{
		std::cout << std::hex << reg << " = " << seconds << "s, expected " << expected << std::endl;
	}
}

void parseMinutesRegister(uint8_t reg, uint8_t expected)
{
	uint8_t minutes = (reg & 0x0F);
	minutes += (((reg & 0xF0) >> 4) * 10);
	if (reg & 0x80) { std::cout << "Non-zero leading bit"; }
	if (minutes != expected)
	{
		std::cout << "0x" << std::hex << reg << " = " << minutes << std::endl;
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
		std::cout << "0x" << std::hex << reg << " = " << hours << std::endl;
	}
	
	if (am != expectedAM)
	{
		std::cout << "Unexpected AMPM bit in 0x" << std::hex << reg << std::endl;
	}
	
	if (hour_mode != expectedHoursBit)
	{
		std::cout << "Unexpected hours bit in 0x" << std::hex << reg << std::endl;
	}
}

void parseDayRegister(uint8_t reg, uint8_t expected)
{
	char const * days[] = {"???", "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
	uint8_t day = (reg & 0x07);
	if (reg & 0xF8) { std::cout << "Non-zero leading bits"; }
	if (day != expected)
	{
		std::cout << "0x" << std::hex << reg << " = " << days[day] << ", expected " << days[expected] << " (" << s << ")" << std::endl;
	}
}

void parseDateRegister(uint8_t reg, uint8_t expected)
{
	uint8_t date = (reg & 0x0F);
	date += (((reg & 0xF0) >> 4) * 10);
	if (reg & 0xC0) { std::cout << "Non-zero leading bits"; }
	if (date != expected)
	{
		std::cout << "Date: 0x" << std::hex << " = " << date << ", expected " << expected << std::endl;
	}
}

void parseMonthRegister(uint8_t reg, uint8_t expected, bool centuryBit)
{
	char const * months[] = {"???", "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
	uint8_t month = (reg & 0x0F);
	month += (((reg & 0x10) >> 4) * 10);
	if (reg & 0x60) { std::cout << "Non-zero leading bits"; }
	if (month != expected)
	{
		std::cout << "0x" << std::hex << " = " << months[month] << " expected " << months[expected] << " (" << s << ")" << std::endl;
	}
	
	if ((bool)(reg & 0x80) ^ (centuryBit))
	{
		std::cout << "Incorrect century bit. Expected " << (centuryBit ? "on" : "off") << " was " << ((reg & 0x80) ? "on" : "off") << std::endl;
	}
}

bool parseYearsRegister(uint8_t reg, uint8_t expected)
{
	uint8_t year = (reg & 0x0F);
	year += (((reg & 0xF0) >> 4) * 10);
	if (year != expected)
	{
		std::cout << "Year 0x" << std::hex << reg << " = " << year << ", expected " << expected << std::endl;
	}
	return (year == expected);
}

int main()
{
	test_All();
	return 0;
}
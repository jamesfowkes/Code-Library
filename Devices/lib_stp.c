/*
 * Standard library includes
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/*
 * Device Library Includes
 */

#include "lib_stp.h"

/* 
 * STP_SendCommand
 * 
 * This function can be used to send a macro-generated command
 * to the printer. It knows about the format of the array generated
 * by the macro, and so hides away the slightly messy use of the first
 * byte for length.
 * 
 * However, it does require the caller to provide a pointer to a
 * a function that can actually transmit the data.
 *
 * The calling code could simply send the array and length straight to 
 * their serial function instead, if desired.
 * 
 * See example in lib_stp_macros.h for details
 */
void STP_SendCommand(uint8_t * command, SERIAL_FN pSerialFn)
{
	if (command && pSerialFn)
	{
		pSerialFn(&command[1], command[0]);
	}
}

void STP_RequestStatus(SERIAL_FN pSerialFn)
{
	uint8_t cmd[] = GET_STATUS;
	pSerialFn(&cmd[1], cmd[0]);
}

bool STP_ParseStatus(PRINTER_STATUS* pStatus, uint8_t * pData)
{

	bool validData = true;

	if (pData)
	{
		validData &= (pData[0] == 'P'); // Status array should start with 'P'
		validData &= (pData[2] == 'V'); // Voltage entry should start with 'V'
		validData &= (pData[5] == 'T'); // Temperature entry should start with 'T'
		validData &= (pStatus != NULL); // Make sure we've got a valid status pointer
	}
	
	if (validData)
	{
		pStatus->paper_ok = (pData[1] == '1');
		
		pStatus->system_voltage_X10 = (pData[3] - '0') * 10; // Convert 1's ASCII digit to decimal (x10)
		pStatus->system_voltage_X10 += (pData[4] - '0'); // Convert 0.1's ASCII digit to decimal (x10)
		
		pStatus->head_temp_celcius = (pData[6] - '0') * 10; // Convert 10's ASCII digit to decimal
		pStatus->head_temp_celcius += (pData[7] - '0'); // Convert 1's ASCII digit to decimal
	}
		
	return validData;
}

void STP_SetOnline(bool online, SERIAL_FN pSerialFn)
{
	uint8_t cmd[] = SET_ONLINE_STATE(online ? 1 : 0);
	pSerialFn(&cmd[1], cmd[0]);	
}

void STP_PrintAndFeedDots(uint8_t nDots, SERIAL_FN pSerialFn)
{
	uint8_t cmd[] = PRINT_AND_FEED_DOTS(nDots);
	pSerialFn(&cmd[1], cmd[0]);	
}

void STP_PrintAndFeedLines(uint8_t nLines, SERIAL_FN pSerialFn)
{
	uint8_t cmd[] = PRINT_AND_FEED_LINES(nLines);
	pSerialFn(&cmd[1], cmd[0]);	
}

bool STP_Setup(uint8_t max_dots, uint8_t heating_time_us, uint8_t heating_interval_us, SERIAL_FN pSerialFn)
{
	bool validData = true;
	
	validData &= (heating_time_us >= 3);
	
	if (validData)
	{
		uint8_t cmd[] = CONTROL_COMMAND(max_dots, heating_time_us, heating_interval_us);
		pSerialFn(&cmd[1], cmd[0]);		
	}
	
	return validData;
}

void STP_SetSleepTime(uint8_t seconds, SERIAL_FN pSerialFn)
{
	uint8_t cmd[] = SLEEP_TIME(seconds);
	pSerialFn(&cmd[1], cmd[0]);
}

void STP_PrintTestPage(SERIAL_FN pSerialFn)
{
	uint8_t cmd[] = PRINT_TEST_PAGE;
	pSerialFn(&cmd[1], cmd[0]);
}

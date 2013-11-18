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
 * STP_SetUserDefinedChar
 * 
 * This function sends a new user-defined char to the printer.
 * It is only responsible for sending the correct header and then the data.
 * The CALLER is required to provide a correctly formatted data array in pData.
 */
void STP_SetUserDefinedChar(uint8_t ascii_code, uint8_t width, uint8_t * pData, SERIAL_FN pSerialFn)
{

	bool bValidInput = true;
	
	bValidInput &= (ascii_code >= 32) && (ascii_code <= 127);
	bValidInput &= (width <= 12);
	
	bValidInput &= (pData != NULL);
	bValidInput &= (pSerialFn != NULL);
	
	if (!bValidInput) { return; }
	
	// Got valid data, send the header
	uint8_t command[] = USER_DEFINED_CHAR_HEADER(ascii_code, ascii_code, width);
	pSerialFn(&command[1], command[0]);
	
	// Then send the data, which should be 3-bytes per width
	pSerialFn(pData, width * 3);
}

void STP_SetUserDefinedCharsState(bool on, SERIAL_FN pSerialFn)
{
	uint8_t cmd[] = SET_USER_DEF_CHARS_STATE(on ? 1 : 0);
	pSerialFn(&cmd[1], cmd[0]);	
}

void STP_DisableUserDefinedChar(uint8_t ascii_char, SERIAL_FN pSerialFn)
{
	uint8_t cmd[] = DISABLE_USER_DEF_CHAR(ascii_char);
	pSerialFn(&cmd[1], cmd[0]);	
}

void STP_SelectCharset(uint8_t charset, SERIAL_FN pSerialFn)
{
	uint8_t cmd[] = SELECT_CHARSET(charset);
	pSerialFn(&cmd[1], cmd[0]);	
}

void STP_SelectCodepage(CODEPAGE codepage, SERIAL_FN pSerialFn)
{
	uint8_t cmd[] = SELECT_CODEPAGE(codepage);
	pSerialFn(&cmd[1], cmd[0]);	
}

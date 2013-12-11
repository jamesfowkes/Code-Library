/*
 * Standard library includes
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

/*
 * Device Library Includes
 */

#include "lib_stp.h"

static void stp_serial_printf(uint8_t * data, uint8_t n)
{
	uint8_t i = 0;
	uint8_t start = 2;
	
	switch (data[0])
	{
	case ESC:
		printf("ESC-");
		printf("%c ", data[1]);
		break;
	case GS:
		printf(" GS-");
		printf("%c ", data[1]);
		break;
	case DC2:
		printf("DC2-");
		printf("%c ", data[1]);
		break;
	default:
		printf("0x%02x, ", data[0]);
		start = 1;
		break;
	}
	
	for (i = start; i < n; ++i)
	{
		printf("0x%02x", data[i]);
		if (i + 1 < n) {printf(", ");}
	}
	printf("\n");
}

int main(int argc, char * argv[])
{
	(void)argc;
	(void)argv;
	
	uint8_t cmd[] = INIT_PRINTER;
	
	uint8_t status_string[] = "P1V72T30";
	PRINTER_STATUS status;
	
	STP_SendCommand(cmd, stp_serial_printf);
	STP_RequestStatus(stp_serial_printf);
	STP_ParseStatus(&status, status_string);
	
	STP_SetOnline(true, stp_serial_printf);
	STP_PrintAndFeedDots(1, stp_serial_printf);
	STP_PrintAndFeedLines(1, stp_serial_printf);
	STP_Setup(127, 127, 127, stp_serial_printf);
	STP_SetSleepTime(10, stp_serial_printf);
	STP_PrintTestPage(stp_serial_printf);

	/* Provided by lib_stp_formatting.c - formatting functionality */
	STP_SetDefaultLineSpacing(stp_serial_printf);
	STP_SetLineSpacing(1, stp_serial_printf);
	STP_SetJustification(JUSTIFY_MIDDLE, stp_serial_printf);
	STP_SetLeftBlankMarginDots(1, 1, stp_serial_printf);
	STP_SetLeftBlankMarginChars(1, stp_serial_printf);
	STP_SetPrintMode(PM_REVERSE_BIT | PM_EMPHASIZED_BIT, stp_serial_printf);
	STP_SetFontEnlarge(0, stp_serial_printf);
	STP_SetBold(true, stp_serial_printf);
	STP_SetDoubleWidth(true, stp_serial_printf);
	STP_SetUpDown(true, stp_serial_printf);
	STP_SetWhtBlkreverse(true, stp_serial_printf);
	STP_SetUnderlineHeight(1, stp_serial_printf);
	STP_SetPrintingDensity(50, 3, stp_serial_printf);

	/* Provided by lib_stp_chars.c - user-defined chars and codeset functionality */
	
	uint8_t charData[] = {
		0xFF, 0xFF, 0xFF,
		0x55, 0x55, 0x55,
		0xAA, 0xAA, 0xAA,
		0xFF, 0xFF, 0xFF
	};
	
	STP_SetUserDefinedChar(50, 4, charData, stp_serial_printf);
	STP_SetUserDefinedCharsState(true, stp_serial_printf);
	STP_DisableUserDefinedChar(50, stp_serial_printf);
	STP_SelectCharset(UK, stp_serial_printf);
	STP_SelectCodepage(PC437, stp_serial_printf);

	/* Provided by lib_stp_barcodes.c - barcode functionality */
	uint8_t barcodeData[] = "www.sparkfun.com";
	STP_PrintBarcode(CODE39, barcodeData, strlen((char*)barcodeData), stp_serial_printf);
	STP_SetBarcodeTextPrintPosition(BELOW_BARCODE, stp_serial_printf);
	STP_SetBarcodeHeight(10, stp_serial_printf);
	STP_SetBarcodeLeftSpace(1, stp_serial_printf);
	STP_SetBarcodeWidth(10, stp_serial_printf);
	
	return 0;
}
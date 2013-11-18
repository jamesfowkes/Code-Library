#ifndef _LIB_SPARKFUNTHERMALPRINTER_H_
#define _LIB_SPARKFUNTHERMALPRINTER_H_

#include "lib_stp_macros.h"

/* 
 * Function prototype that the library expects to 
 * let it transmit data over serial 
 *
 * This is kept deliberately simple to avoid implementation detail.
 *
 * Calling code will probably have to provide a small interface function
 * with this prototype that will call the implemetation's own serial function.
 */
typedef void (*SERIAL_FN)(uint8_t * data, uint8_t n);

/* Struct that will be filled with printer status on receipt.
 * Used by STP_GetStatus function, if built in
 */
struct printer_status
{
	bool paper_ok;
	uint8_t system_voltage_X10;
	uint8_t head_temp_celcius;
};
typedef struct printer_status PRINTER_STATUS;

/* 
 * In order to save memory on constrained platforms, the functionality
 * for the thermal printer is split into several modules.
 *
 * None of these modules HAVE to be built.
 * If your compiler supports it, you can try to optimize out unused functions anyway.
 *
 * The macros in lib_stp_macros.h can be used directly if preferred.
 *
 * Each function takes, as a mimimum, a pointer to a function that will output serial
 * commands to the printer
 */

/* Provided by lib_stp.c - generic functionality */
void STP_SendCommand(uint8_t * command, SERIAL_FN pSerialFn);
void STP_RequestStatus(SERIAL_FN pSerialFn);
bool STP_ParseStatus(PRINTER_STATUS* pStatus, uint8_t * pData);
void STP_SetOnline(bool online, SERIAL_FN pSerialFn);
void STP_PrintAndFeedDots(uint8_t nDots, SERIAL_FN pSerialFn);
void STP_PrintAndFeedLines(uint8_t nLines, SERIAL_FN pSerialFn);
bool STP_Setup(uint8_t max_dots, uint8_t heating_time_us, uint8_t heating_interval_us, SERIAL_FN pSerialFn);
void STP_SetSleepTime(uint8_t seconds, SERIAL_FN pSerialFn);
void STP_PrintTestPage(SERIAL_FN pSerialFn);

/* Provided by lib_stp_formatting.c - formatting functionality */
void STP_SetDefaultLineSpacing(SERIAL_FN pSerialFn);
void STP_SetLineSpacing(uint8_t spacing, SERIAL_FN pSerialFn);
void STP_SetJustification(JUSTIFICATION justification, SERIAL_FN pSerialFn);
void STP_SetLeftBlankMarginDots(uint8_t l, uint8_t h, SERIAL_FN pSerialFn);
void STP_SetLeftBlankMarginChars(uint8_t chars, SERIAL_FN pSerialFn);
void STP_SetPrintMode(uint8_t mode, SERIAL_FN pSerialFn);
void STP_SetFontEnlarge(uint8_t setting, SERIAL_FN pSerialFn);
void STP_SetBold(bool bold, SERIAL_FN pSerialFn);
void STP_SetDoubleWidth(bool set, SERIAL_FN pSerialFn);
void STP_SetUpDown(bool updown, SERIAL_FN pSerialFn);
void STP_SetWhtBlkreverse(bool reverse, SERIAL_FN pSerialFn);
void STP_SetUnderlineHeight(uint8_t height, SERIAL_FN pSerialFn);
void STP_SetPrintingDensity(uint8_t density, uint8_t break_time, SERIAL_FN pSerialFn);

/* Provided by lib_stp_chars.c - user-defined chars and codeset functionality */
void STP_SetUserDefinedChar(uint8_t ascii_code, uint8_t width, uint8_t * pData, SERIAL_FN pSerialFn);
void STP_SetUserDefinedCharsState(bool on, SERIAL_FN pSerialFn);
void STP_DisableUserDefinedChar(uint8_t ascii_char, SERIAL_FN pSerialFn);
void STP_SelectCharset(uint8_t charset, SERIAL_FN pSerialFn);
void STP_SelectCodepage(CODEPAGE codepage, SERIAL_FN pSerialFn);

/* Provided by lib_stp_barcodes.c - barcode functionality */
bool STP_PrintBarcode(BARCODE_SYMBOLOGY symb, uint8_t * pData, uint8_t n, SERIAL_FN pSerialFn);
void STP_SetBarcodeTextPrintPosition(BARCODE_PRINT_POSITION position, SERIAL_FN pSerialFn);
void STP_SetBarcodeHeight(uint8_t height, SERIAL_FN pSerialFn);
void STP_SetBarcodeLeftSpace(uint8_t space, SERIAL_FN pSerialFn);
void STP_SetBarcodeWidth(uint8_t width, SERIAL_FN pSerialFn);

#endif

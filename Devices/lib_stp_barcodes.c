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

bool STP_PrintBarcode(BARCODE_SYMBOLOGY symb, uint8_t * pData, uint8_t n, SERIAL_FN pSerialFn)
{
	bool validData = true;
	
	validData &= (pData != NULL);
	
	/* First test for correct number of chars, n */
	switch (symb)
	{
	case UPCA:
	case UPCE:
		// For these symbologies, n must be 11 or 12
		validData &= ((n == 11) || (n == 12));
		break;
	case EAN13:
		// For EAN13, n must be 12 or 13
		validData &= ((n == 12) || (n == 13));
		break;
	case EAN8:
		// For EAN8, n must be 7 or 8
		validData &= ((n == 12) || (n == 13));
		break;
	case I25:
		// I25 must be even number of chars, > 1
		validData &= ((n & 0x01) == 0);
		validData &= (n > 1);
		break;
	case CODE39:
	case CODEBAR:
	case CODE93:
	case CODE128:
	case CODE11:
	case MSI:
		// All other symbologies, n > 1
		validData &= (n > 1);
		break;
	}
	
	if (validData)
	{
		uint8_t cmd[] = BARCODE_HEADER_N(symb, n); // Use format where number of chars is specified
		pSerialFn(&cmd[1], cmd[0]);	// Command header
		pSerialFn(pData, n); // Data
	}
	
	return validData;
}

void STP_SetBarcodeTextPrintPosition(BARCODE_PRINT_POSITION position, SERIAL_FN pSerialFn)
{
	uint8_t cmd[] = SET_BARCODE_PRINT_POSITION(position);
	pSerialFn(&cmd[1], cmd[0]);	
}

void STP_SetBarcodeHeight(uint8_t height, SERIAL_FN pSerialFn)
{
	uint8_t cmd[] = SET_BARCODE_HEIGHT(height);
	pSerialFn(&cmd[1], cmd[0]);	
}

void STP_SetBarcodeLeftSpace(uint8_t space, SERIAL_FN pSerialFn)
{
	uint8_t cmd[] = SET_BARCODE_LEFT_SPACE(space);
	pSerialFn(&cmd[1], cmd[0]);	
}

void STP_SetBarcodeWidth(uint8_t width, SERIAL_FN pSerialFn)
{
	uint8_t cmd[] = SET_BARCODE_WIDTH(width);
	pSerialFn(&cmd[1], cmd[0]);	
}




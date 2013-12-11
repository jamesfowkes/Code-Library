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

void STP_SetDefaultLineSpacing(SERIAL_FN pSerialFn)
{
	uint8_t cmd[] = SET_DEFAULT_LINE_SPACING;
	pSerialFn(&cmd[1], cmd[0]);	
}

void STP_SetLineSpacing(uint8_t spacing, SERIAL_FN pSerialFn)
{
	uint8_t cmd[] = SET_LINE_SPACING(spacing);
	pSerialFn(&cmd[1], cmd[0]);	
}

void STP_SetJustification(JUSTIFICATION justification, SERIAL_FN pSerialFn)
{
	uint8_t cmd[] = SET_JUSTIFICATION(justification);
	pSerialFn(&cmd[1], cmd[0]);	
}

void STP_SetLeftBlankMarginDots(uint8_t l, uint8_t h, SERIAL_FN pSerialFn)
{
	uint8_t cmd[] = SET_LEFT_BLANK_MARGIN(l, h);
	pSerialFn(&cmd[1], cmd[0]);	
}

void STP_SetLeftBlankMarginChars(uint8_t chars, SERIAL_FN pSerialFn)
{
	uint8_t cmd[] = SET_LEFT_BLANK_CHARS(chars);
	pSerialFn(&cmd[1], cmd[0]);	
}

void STP_SetPrintMode(uint8_t mode, SERIAL_FN pSerialFn)
{
	uint8_t cmd[] = SET_PRINT_MODE(mode);
	pSerialFn(&cmd[1], cmd[0]);	
}

void STP_SetFontEnlarge(uint8_t setting, SERIAL_FN pSerialFn)
{
	uint8_t cmd[] = SET_FONT_ENLARGE(setting);
	pSerialFn(&cmd[1], cmd[0]);	
}
 
void STP_SetBold(bool bold, SERIAL_FN pSerialFn)
{
	uint8_t cmd[] = SET_BOLD_STATE(bold ? 1 : 0);
	pSerialFn(&cmd[1], cmd[0]);	
}

void STP_SetDoubleWidth(bool set, SERIAL_FN pSerialFn)
{
	uint8_t cmd_on[] = SET_DOUBLE_WIDTH_ON;
	uint8_t cmd_off[] = SET_DOUBLE_WIDTH_OFF;
	
	if (set)
	{
		pSerialFn(&cmd_on[1], cmd_on[0]);	
	}
	else
	{
		pSerialFn(&cmd_off[1], cmd_off[0]);	
	}
}

void STP_SetUpDown(bool updown, SERIAL_FN pSerialFn)
{
	uint8_t cmd[] = SET_UPDOWN_STATE(updown ? 1 : 0);
	pSerialFn(&cmd[1], cmd[0]);	
}

void STP_SetWhtBlkreverse(bool reverse, SERIAL_FN pSerialFn)
{
	uint8_t cmd[] = SET_WH_BK_REVERSE_STATE(reverse ? 1 : 0);
	pSerialFn(&cmd[1], cmd[0]);	
}

void STP_SetUnderlineHeight(uint8_t height, SERIAL_FN pSerialFn)
{
	uint8_t cmd[] = SET_UNDERLINE_HEIGHT(height);
	pSerialFn(&cmd[1], cmd[0]);	
}

void STP_SetPrintingDensity(uint8_t density, uint8_t break_time, SERIAL_FN pSerialFn)
{
	uint8_t cmd[] = SET_DENSITY_AND_BREAK(density, break_time);
	pSerialFn(&cmd[1], cmd[0]);	
}

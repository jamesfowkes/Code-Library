#ifndef _LIB_SPARKFUNTHERMALPRINTER_MACROS_H_
#define _LIB_SPARKFUNTHERMALPRINTER_MACROS_H_

/* To save filling up memory with all the commands of the
thermal printer, only a fraction of which might be used,
they are #define here. To use, simply declare an array of bytes
and initialise it to the desired command with the appropriate macro.

The length of the command is in the first byte of the generated array.
They cannot be NULL-terminated, as there may be valid 0's in the data.

Therefore, user must check the first byte to determine how many chars to transmit.

For example, to set line spacing to 4:

uint8_t lineSpacingCommand[] = SET_LINE_SPACING(4);

// lineSpacingCommand[1] is first byte of data
// lineSpacingCommand[0] is length

serial_print(&lineSpacingCommand[1], lineSpacingCommand[0]]) <-- Your own system's serial function
OR
STP_SendCommand(lineSpacingCommand, serial_print) <-- Let the library handle it via function pointer

*/

/* ASCII non-printable symbols */
/****************************************************************/
#define HT (9)
#define LF (10)
#define FF (12)
#define SO (14)
#define DC2 (18)
#define DC4 (20)
#define ESC (27)
#define GS (29)
/****************************************************************/

/* 
 * Macros for creating command arrays
 */
/****************************************************************/
#define ESC_CMD(x) {2, ESC, (x)}
#define ESC_CMD_N(x,n) {3, ESC, (x), (n)}
#define ESC_CMD_3N(x,n1,n2,n3) {5, ESC, (x), (n1), (n2), (n3)}
#define ESC_CMD_4N(x,n1,n2,n3,n4) {6, ESC, (x), (n1), (n2), (n3), (n4)}

#define GS_CMD_N(x, n) {3, GS, (x), (n)}
#define GS_CMD_2N(x, n1, n2) {4, GS, (x), (n1), (n2)}

#define DC2_CMD(x) {2, DC2, (x)}
#define DC2_CMD_N(x,n) {3, DC2, (x), (n)}
/****************************************************************/

/*
 * Printer Commands
 */

#define INIT_PRINTER ESC_CMD('@')
#define GET_STATUS ESC_CMD('v')

#define PRINT_AND_FEED_DOTS(x) ESC_CMD_N('J', x)
#define PRINT_AND_FEED_LINES(x) ESC_CMD_N('d', x)

#define GO_OFFLINE ESC_CMD_N('=', 0)
#define GO_ONLINE ESC_CMD_N('=', 1)
#define SET_ONLINE_STATE(x) ESC_CMD_N('=', x)

#define SET_DEFAULT_LINE_SPACING ESC_CMD('2')
#define SET_LINE_SPACING(x) ESC_CMD_N('3',x)

enum justification
{
	JUSTIFY_LEFT,
	JUSTIFY_MIDDLE,
	JUSTIFY_RIGHT
};
typedef enum justification JUSTIFICATION;

#define SET_JUSTIFICATION(x) ESC_CMD_N('a',x)

#define SET_LEFT_BLANK_MARGIN(nL, nH) GS_CMD_2N('L', nL, nH)
#define SET_LEFT_BLANK_CHARS(n) ESC_CMD_N('B', n)

/* Defines for setting print mode bitfield */
// Bit0 is always 0
#define PM_REVERSE_BIT			(1 << 1)
#define PM_UPDOWN_BIT			(1 << 2)
#define PM_EMPHASIZED_BIT		(1 << 3)
#define PM_DOUBLE_HEIGHT_BIT	(1 << 4)
#define PM_DOUBLE_WIDTH_BIT		(1 << 5)
#define PM_DELETE_LINE_BIT		(1 << 6)
// Bit7 is always 0

#define SET_PRINT_MODE(mode) ESC_CMD_N('!', mode)

/* Defines for font enlarge */
#define FE_HEIGHT_ENLARGE	(1 << 0)
#define FE_WIDTH_ENLARGE	(1 << 4)
#define SET_FONT_ENLARGE(setting) GS_CMD_N('!', setting)

#define SET_BOLD_OFF ESC_CMD_N('E', 0)
#define SET_BOLD_ON ESC_CMD_N('E', 1)
#define SET_BOLD_STATE(x) ESC_CMD_N('E', x)

#define SET_DOUBLE_WIDTH_OFF ESC_CMD(DC4)
#define SET_DOUBLE_WIDTH_ON ESC_CMD(SO)

#define SET_UPDOWN_OFF ESC_CMD_N('{', 0)
#define SET_UPDOWN_ON ESC_CMD_N('{', 0)
#define SET_UPDOWN_STATE(x) ESC_CMD_N('{', x)

#define SET_WH_BK_REVERSE_OFF GS_CMD_N('B', 0)
#define SET_WH_BK_REVERSE_ON GS_CMD_N('B', 0)
#define SET_WH_BK_REVERSE_STATE(x) GS_CMD_N('B', x)

#define SET_UNDERLINE_HEIGHT(n) ESC_CMD_N('-', n)

#define SET_USER_DEF_CHARS_OFF ESC_CMD_N('%', 0)
#define SET_USER_DEF_CHARS_ON ESC_CMD_N('%', 0)
#define SET_USER_DEF_CHARS_STATE(x) ESC_CMD_N('%', x)

#define USER_DEFINED_CHAR_HEADER(n, m, w) ESC_CMD_4N('&', 3, n, m, w)

#define DISABLE_USER_DEF_CHAR(n) ESC_CMD_N('?', n)

/* Define charset numbers */
enum charset
{
	USA,
	FRANCE,
	GERMANY,
	UK,
	DENMARK_1,
	SWEDEN,
	ITALY,
	SPAIN_1,
	JAPAN,
	NORWAY,
	DENMARK_2,
	SPAIN_2,
	LATIN_AMERICA,
	KOREA
};
typedef enum charset CHARSET;

#define SELECT_CHARSET(n) ESC_CMD_N('R', n)

/* Define charater code pages */
enum codepage
{
	PC437,
	PC850
};
typedef enum codepage CODEPAGE;

#define SELECT_CODEPAGE(x) ESC_CMD_N('t', x)

/* Automatic-status-back defines */
#define ASB_ENABLE		(1 << 2)
#define ASB_RTS_ENABLE	(1 << 5)

#define ASB_SET(x) GS_CMD_N('a', x)

/* Human-readable barcode position */
enum barcode_print_position
{
	NOT_PRINTED,
	ABOVE_BARCODE,
	BELOW_BARCODE,
	ABOVE_BELOW_BARCODE
};
typedef enum barcode_print_position BARCODE_PRINT_POSITION;

#define SET_BARCODE_PRINT_POSITION(pos) GS_CMD_N('H', pos)

#define SET_BARCODE_HEIGHT(height) GS_CMD_N('h', height)

#define SET_BARCODE_LEFT_SPACE(space) GS_CMD_N('x', space)

#define SET_BARCODE_WIDTH(width) GS_CMD_N('w', width)

enum barcode_symbology
{
	UPCA,
	UPCE,
	EAN13,
	EAN8,
	CODE39,
	I25,
	CODEBAR,
	CODE93,
	CODE128,
	CODE11,
	MSI
};
typedef enum barcode_symbology BARCODE_SYMBOLOGY;

#define BARCODE_HEADER(symb) GS_CMD_N('k', symb)
#define BARCODE_HEADER_N(symb, n) GS_CMD_2N('k', symb+65, n)

#define CONTROL_COMMAND(max_dots, heating_time, heating_interval) \
	ESC_CMD_3N('7', max_dots, heating_time, heating_interval)
	
#define SLEEP_TIME(time) ESC_CMD_N('8', time)

#define SET_DENSITY_AND_BREAK(density, break) DC2_CMD_N('#', ((break & 0x07) << 5) | (density & 0x1F))

#define PRINT_TEST_PAGE DC2_CMD('T')

#endif
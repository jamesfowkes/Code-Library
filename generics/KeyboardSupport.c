/*
 * Standard Library Includes
 */
#include <ctype.h>
#include <stdint.h>
#include <stdbool.h>

/* LUFA includes  - required for scancode definitions*/
#include <LUFA/Drivers/USB/USB.h>

#include "KeyboardSupport.h"

#define SCANCODE_TO_ASCII_LOWERCASE(scancode) (scancode + 93)
#define SCANCODE_TO_ASCII_UPPERCASE(scancode) (scancode + 61)

#define ASCII_LOWERCASE_TO_SCANCODE(ascii) (ascii - 93)
#define ASCII_UPPERCASE_TO_SCANCODE(ascii) (ascii - 61)

#define NUMERIC_TO_SCANCODE(number) (number > 0 ? number - 19 : HID_KEYBOARD_SC_0_AND_CLOSING_PARENTHESIS)
#define SCANCODE_TO_NUMERIC(scancode) (scancode > HID_KEYBOARD_SC_0_AND_CLOSING_PARENTHESIS ? scancode + 19 : '0')

struct asciiToScancodeMap
{
	uint8_t ascii;
	uint8_t scancode;
	bool	needsShiftModifier;
};
typedef struct asciiToScancodeMap ASCIITOSCANCODEMAP;

static ASCIITOSCANCODEMAP NonAlphaNumeric[] = {
	{'!',	HID_KEYBOARD_SC_1_AND_EXCLAMATION,					true},
	{'"',	HID_KEYBOARD_SC_2_AND_AT,							true},
	{'$',	HID_KEYBOARD_SC_4_AND_DOLLAR,						true},
	{'%',	HID_KEYBOARD_SC_5_AND_PERCENTAGE,					true},
	{'^',	HID_KEYBOARD_SC_6_AND_CARET,						true},
	{'&',	HID_KEYBOARD_SC_7_AND_AND_AMPERSAND,				true},
	{'*',	HID_KEYBOARD_SC_8_AND_ASTERISK,						true},
	{'(',	HID_KEYBOARD_SC_9_AND_OPENING_PARENTHESIS,			true},
	{')',	HID_KEYBOARD_SC_0_AND_CLOSING_PARENTHESIS,			true},

	{' ',	HID_KEYBOARD_SC_SPACE,								false},
	{'\n',	HID_KEYBOARD_SC_ENTER,								false},

	{'-',	HID_KEYBOARD_SC_MINUS_AND_UNDERSCORE,				false},
	{'_',	HID_KEYBOARD_SC_MINUS_AND_UNDERSCORE,				true},

	{'=',	HID_KEYBOARD_SC_EQUAL_AND_PLUS,						false},
	{'+',	HID_KEYBOARD_SC_EQUAL_AND_PLUS,						true},

	{'[',	HID_KEYBOARD_SC_OPENING_BRACKET_AND_OPENING_BRACE,	false},
	{'{',	HID_KEYBOARD_SC_OPENING_BRACKET_AND_OPENING_BRACE,	true},

	{']',	HID_KEYBOARD_SC_CLOSING_BRACKET_AND_CLOSING_BRACE,	false},
	{'}',	HID_KEYBOARD_SC_CLOSING_BRACKET_AND_CLOSING_BRACE,	true},

	{'\\',	HID_KEYBOARD_SC_BACKSLASH_AND_PIPE,					false},
	{'|',	HID_KEYBOARD_SC_BACKSLASH_AND_PIPE,					true},

	{'\'',	HID_KEYBOARD_SC_APOSTROPHE_AND_QUOTE,				false},
	{'@',	HID_KEYBOARD_SC_APOSTROPHE_AND_QUOTE,				true},

	{';',	HID_KEYBOARD_SC_SEMICOLON_AND_COLON,				false},
	{':',	HID_KEYBOARD_SC_SEMICOLON_AND_COLON,				true},

	{',',	HID_KEYBOARD_SC_COMMA_AND_LESS_THAN_SIGN,			false},
	{'<',	HID_KEYBOARD_SC_COMMA_AND_LESS_THAN_SIGN,			true},

	{'.',	HID_KEYBOARD_SC_DOT_AND_GREATER_THAN_SIGN,			false},
	{'>',	HID_KEYBOARD_SC_DOT_AND_GREATER_THAN_SIGN,			true},

	{'/',	HID_KEYBOARD_SC_SLASH_AND_QUESTION_MARK,			false},
	{'?',	HID_KEYBOARD_SC_SLASH_AND_QUESTION_MARK,			true},
};

static void NonAlphaSearch(uint8_t ascii, uint8_t * pScancode, uint8_t * pModifier);

void asciiToScancode(uint8_t ascii, uint8_t * pScancode, uint8_t * pModifier)
{

	// Assume no modifiers
	*pModifier = 0;

	if ( islower(ascii) )
	{
		*pScancode = ASCII_LOWERCASE_TO_SCANCODE(ascii);
	}
	else if ( isupper(ascii) )
	{
		*pModifier = HID_KEYBOARD_MODIFIER_LEFTSHIFT;
		*pScancode = ASCII_UPPERCASE_TO_SCANCODE(ascii);
	}
	else if ( isdigit(ascii) )
	{
		*pScancode = NUMERIC_TO_SCANCODE(ascii);
	}
	else
	{
		NonAlphaSearch(ascii, pScancode, pModifier);
	}
}

static void NonAlphaSearch(uint8_t ascii, uint8_t * pScancode, uint8_t * pModifier)
{

	uint8_t i = 0;

	for (; i < sizeof(NonAlphaNumeric); ++i)
	{
		if (NonAlphaNumeric[i].ascii == ascii)
		{
			*pScancode = NonAlphaNumeric[i].scancode;

			if (NonAlphaNumeric[i].needsShiftModifier)
			{
				*pModifier = HID_KEYBOARD_MODIFIER_LEFTSHIFT;
			}
			return;
		}
	}
}

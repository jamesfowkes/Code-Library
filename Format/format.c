/*
 * Standard Library Includes
 */
 
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/*
 * Format Library Includes
 */

#include "format.h"

/*
 * Private Function Prototypes
 */

static uint8_t format(char * buf, const char spec, void * pArg);
static uint8_t getPlaceCount(uint32_t value, uint32_t place);

/*
 * Public Function Definitions
 */
 
void TranslateBuffer(char const * const inBuffer, char * outBuffer, uint8_t size, void * args[], uint8_t nargs)
{
	uint8_t i = 0;	//Index into user buffer
	uint8_t tx = 0;	//Index into transmit buffer
	uint8_t arg = 0;	//Index into argument array

	if (size == 0)
	{
		size = strlen((char*)inBuffer);
	}

	if (size)
	{
		do
		{
			if ((inBuffer[i] == '%') && (i+1 < size) && (nargs > 0))
			{
				tx += format(&outBuffer[tx], inBuffer[i+1], args[arg++]);
				++i; // Skip formatting character
			}
			else
			{
				outBuffer[tx++] = inBuffer[i];
			}
		} while (++i < size);
	}
	outBuffer[tx++] = '\r';
	outBuffer[tx++] = '\n';
	outBuffer[tx] = '\0';
}

/*
 * Private Function Definitions
 */
 
/* format
 : Convert a single integer argument (pArg) into a string.
 : Write into buf
 : Size and type of integer specified by spec
 */
static uint8_t format(char * buf, const char spec, void * pArg)
{
	uint8_t bytesWritten = 0;

	uint32_t uVal = 0;
	int32_t sVal = 0;
	uint8_t digitcount = 0;
	bool foundSignificantDigit = false;

	switch(spec)
	{
	case 'u':// Unsigned 8-bit int
		uVal = (uint32_t)( *( (uint8_t *)pArg ) );
		break;
	case 'U':// Unsigned 16-bit int
		uVal = (uint32_t)( *( (uint16_t *)pArg ) );
		break;
	case 's':// Signed 8-bit int
		sVal = (int32_t) ( *( (int8_t *)pArg ) );
		uVal = labs(sVal);
		break;
	case 'S':// Signed 16-bit int
		sVal = (int32_t) ( *( (int16_t *)pArg ) );
		uVal = labs(sVal);
		break;
	case 'L':// Signed 32-bit int
		uVal = (uint32_t)( *( (uint32_t *)pArg ) );
		break;
	}

	if (sVal < 0)
	{
		*buf++ = '-';
		bytesWritten++;
	}

	uint32_t placevalue = 0;

	for (placevalue = 1000000000; placevalue > 1; placevalue /= 10)
	{
		digitcount = getPlaceCount(uVal, placevalue);
		if (digitcount || foundSignificantDigit)
		{
			foundSignificantDigit = true;
			*buf++ = (char)(digitcount + '0');
			uVal -= (digitcount * placevalue);
			bytesWritten++;
		}
	}

	digitcount = uVal;
	*buf++ = (char)(digitcount + '0');
	bytesWritten++;

	return bytesWritten;
}

/* getPlaceCount
 : Get the magnitude at the specified digit in a number
 */
static uint8_t getPlaceCount(uint32_t value, uint32_t place)
{
	if (place > 1 && value > 0)
	{
		return (uint8_t)(value / place);
	}
	else
	{
		return value - (10 * (value / 10));
	}
}

/*
 * Standard Library Includes
 */
 
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

/*
 * Format Library Includes
 */

#include "integertypeenum.h"
#include "format.h"

/*
 * Defines and Typedefs
 */
 
#ifdef FORMAT_32_BIT_INTS
typedef uint32_t CONVERSION_TYPE;
typedef int32_t SIGNED_CONVERSION_TYPE;
#else
typedef uint16_t CONVERSION_TYPE;
typedef int16_t SIGNED_CONVERSION_TYPE;
#endif

/*
 * Private Function Prototypes
 */

static inline uint8_t formatSingleArg(char * buf, const char spec, void * pArg);
static inline uint8_t getPlaceCount(CONVERSION_TYPE value, CONVERSION_TYPE place);

static uint8_t signedToString(CONVERSION_TYPE places, SIGNED_CONVERSION_TYPE val, char * buf);
static uint8_t toString(CONVERSION_TYPE places, CONVERSION_TYPE val, char * buf);

/*
 * Public Function Definitions
 */
 
uint8_t TranslateBuffer(char const * const inBuffer, char * outBuffer, uint8_t size, void * args[], uint8_t nargs, bool nullTerminate)
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
				tx += formatSingleArg(&outBuffer[tx], inBuffer[i+1], args[arg++]);
				++i; // Skip formatting character
			}
			else
			{
				outBuffer[tx++] = inBuffer[i];
			}
		} while (++i < size);
	}
	
	if (nullTerminate)
	{
		outBuffer[tx] = '\0'; // Do not increment the character count for null terminator
	}
	
	return tx;
}

uint8_t IntToString(void * pVal, char * buf, INTEGERTYPE eType, bool rightAlign, uint8_t bufferLength)
{
	(void)rightAlign;
	(void)bufferLength;
	
	uint8_t bytesWritten = 0;
	
	if (pVal)
	{
		switch(eType)
		{
		case S8:
			bytesWritten = signedToString(100, (CONVERSION_TYPE)*(int8_t*)pVal, buf);
			break;
		case U8:
			bytesWritten = toString(100, (CONVERSION_TYPE)*(uint8_t*)pVal, buf);
			break;
		case S16:
			bytesWritten = S16ToString(*(int16_t*)pVal, buf);
			break;
		case U16:
			bytesWritten = toString(10000, (CONVERSION_TYPE)*(uint16_t*)pVal, buf);
			break;
		#ifdef FORMAT_32_BIT_INTS
		case S32:
			bytesWritten = S32ToString(*(int32_t*)pVal, buf);
			break;
		case U32:
			bytesWritten = toString(1000000000, (CONVERSION_TYPE)*(uint32_t*)pVal, buf);
			break;
		#endif
		default:
			break;
		}
	}
	
	return bytesWritten;
}

uint8_t S8ToString(int8_t val, char * buf)
{
	return signedToString(100, (SIGNED_CONVERSION_TYPE)val, buf);
}

uint8_t U8ToString(uint8_t val, char * buf)
{
	return toString(100, (CONVERSION_TYPE)val, buf);
}

uint8_t S16ToString(int16_t val, char * buf)
{
	return signedToString(10000, (SIGNED_CONVERSION_TYPE)val, buf);
}

uint8_t U16ToString(uint16_t val, char * buf)
{
	return toString(10000, (CONVERSION_TYPE)val, buf);
}

#ifdef FORMAT_32_BIT_INTS
uint8_t S32ToString(int32_t val, char * buf)
{
	return signedToString(1000000000, (SIGNED_CONVERSION_TYPE)val, buf);
}

uint8_t U32ToString(uint32_t val, char * buf)
{
	return toString(1000000000, (CONVERSION_TYPE)val, buf);
}
#endif

/*
 * Private Function Definitions
 */
 
/* format
 : Convert a single integer argument (pArg) into a string.
 : Write into buf
 : Size and type of integer specified by spec
 */
static inline uint8_t formatSingleArg(char * buf, const char spec, void * pArg)
{
	uint8_t bytesWritten = 0;

	switch(spec)
	{
	case 'u':// Unsigned 8-bit int
		bytesWritten = toString(100, (CONVERSION_TYPE)*(uint8_t*)pArg, buf);
		break;
	case 'U':// Unsigned 16-bit int
		bytesWritten = toString(10000, (CONVERSION_TYPE)*(uint16_t*)pArg, buf);
		break;
	case 's':// Signed 8-bit int
		bytesWritten = signedToString(100, (SIGNED_CONVERSION_TYPE)*(int8_t*)pArg, buf);
		break;
	case 'S':// Signed 16-bit int
		bytesWritten = signedToString(10000, (SIGNED_CONVERSION_TYPE)*(int16_t*)pArg, buf);
		break;
	#ifdef FORMAT_32_BIT_INTS
	case 'L':// Signed 32-bit int
		bytesWritten = signedToString(1000000000, (SIGNED_CONVERSION_TYPE)*(int32_t*)pArg, buf);
		break;
	#endif
	default:
		break;
	}
	
	return bytesWritten;
}

static uint8_t signedToString(CONVERSION_TYPE places, SIGNED_CONVERSION_TYPE val, char * buf)
{
	if (val < 0)
	{
		*buf++ = '-';
		return toString(places, abs(val), buf) + 1;
	}
	else
	{
		return toString(places, val, buf);
	}
}

static uint8_t toString(CONVERSION_TYPE places, CONVERSION_TYPE val, char * buf)
{
	CONVERSION_TYPE placevalue = 0;
	uint8_t digitcount = 0;
	bool foundSignificantDigit = false;
	uint8_t bytesWritten = 0;
	
	for (placevalue = places; placevalue > 1; placevalue /= 10)
	{
		digitcount = getPlaceCount(val, placevalue);
		if (digitcount || foundSignificantDigit)
		{
			foundSignificantDigit = true;
			*buf++ = (char)(digitcount + '0');
			val -= (digitcount * placevalue);
			bytesWritten++;
		}
	}
	
	digitcount = val;
	*buf++ = (char)(digitcount + '0');
	bytesWritten++;
	
	return bytesWritten;
}

/* getPlaceCount
 : Get the magnitude at the specified digit in a number
 */
static inline uint8_t getPlaceCount(CONVERSION_TYPE value, CONVERSION_TYPE place)
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

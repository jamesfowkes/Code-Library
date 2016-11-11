/*
 * util_simple_parse.c
 *
 * Routines for simple parsing of short string to ints, with range checking
 *
 * Probably smaller and faster than native C library functions for small strings
 */

/*
 * C Library Includes
 */

#include <stdbool.h>
#include <stdint.h>

/*
 * Public Functions
 */

/*bool parse_char_to_int(int * pResult, char to_parse, int count, int * range = NULL)
{
    if (!pResult) { return false; }

    bool valid = true;

    int result = to_parse - '0';

    if (range)
    {
        valid &= result >= range[0];
        valid &= result <= range[1];
    }
    else
    {
        valid &= result >= 0;
        valid &= result <= 9; 
    }

    if (valid)
    {
        *pResult = result;
    }

    return valid;
}*/

bool parse_chars_to_int(int * pResult, char const * const number, int count, int * range)
{
    // Convert ASCII chars to actual value

    if (!pResult || !number) { return false; }

    int result = 0; 
    int max = 1; // Max will be set to 9 if count is 1, 99 if count is 2 etc...
    int c;

    for (c = 0; c < count; c++)
    {
        result *= 10;
        result += number[c] - '0';
        max *= 10; // Max goes 10, 100, 1000...
    }

    max--; // Decrementing max makes it 9, 99, 999...
        
    bool valid = true;
    
    if (range)
    {
        valid &= result >= range[0];
        valid &= result <= range[1];
    }
    else
    {
        valid &= result >= 0;
        valid &= result <= max;
    }

    if (valid)
    {
        *pResult = result;
    }

    return valid;
}
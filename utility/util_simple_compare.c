/*
 * util_simple_compare.c
 *
 * Routines for simple equality comparison of small strings without using strcmp etc.
 *
 * Probably smaller and faster than native C library functions for small strings if only equality is required.
 */

/*
 * C Library Includes
 */

#include <stdbool.h>
#include <stdint.h>

/*
 * Public Functions
 */

bool simple_cmp(int count, char const * const p1, char const * const p2)
{
    // Returns TRUE is p1 and p2 are the same to the 3rd char
    if (!p1 || !p2) { return false; }
 
    bool same = true;
    while(count--)
    {
    	same &= p1[count] == p2[count];
    }
    return same;
}

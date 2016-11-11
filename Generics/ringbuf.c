/*####COPYRIGHTBEGIN####
-------------------------------------------
Copyright (C) 2008 by Steve Karg

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to:
The Free Software Foundation, Inc.
59 Temple Place - Suite 330
Boston, MA  02111-1307
USA.

As a special exception, if other files instantiate templates or
use macros or inline functions from this file, or you compile
this file and link it with other works to produce a work based
on this file, this file does not by itself cause the resulting
work to be covered by the GNU General Public License. However
the source code for this file must still be made available in
accordance with section (3) of the GNU General Public License.

This exception does not invalidate any other reasons why a work
based on this file might be covered by the GNU General Public
License.
-------------------------------------------
####COPYRIGHTEND####*/

/* Functional Description: Generic ring buffer library for deeply
embedded system. See the unit tests for usage examples. */

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>

#include "ringbuf.h"
#include "util_macros.h"

static uint8_t * get_element_n(RING_BUFFER *b, uint8_t n);
static uint8_t get_element_n_index(RING_BUFFER *b, uint8_t n);
static unsigned valid_power_of_two(unsigned k);

/****************************************************************************
* DESCRIPTION: Returns the number of elements in the ring buffer
* RETURN:      Number of elements in the ring buffer
* ALGORITHM:   none
* NOTES:       none
*****************************************************************************/
unsigned ringbuf_count (
RING_BUFFER const *b)
{
	return (b ? min((b->head - b->tail), b->element_count) : 0);
}

/****************************************************************************
* DESCRIPTION: Returns the empty/full status of the ring buffer
* RETURN:      true if the ring buffer is full, false if it is not.
* ALGORITHM:   none
* NOTES:       none
*****************************************************************************/
bool ringbuf_full (
RING_BUFFER const *b)
{
	return (b ? ((b->head - b->tail) >= b->element_count) : true);
}

/****************************************************************************
* DESCRIPTION: Returns the empty/full status of the ring buffer
* RETURN:      true if the ring buffer is empty, false if it is not.
* ALGORITHM:   none
* NOTES:       none
*****************************************************************************/
bool ringbuf_empty(
RING_BUFFER const *b)
{
	return (b ? (ringbuf_count(b) == 0) : true);
}

/****************************************************************************
* DESCRIPTION: Looks at the data from the head of the list without removing it
* RETURN:      pointer to the data, or NULL if nothing in the list
* ALGORITHM:   none
* NOTES:       none
*****************************************************************************/
uint8_t *ringbuf_get_oldest(
RING_BUFFER const *b)
{
	if (b) {
		return (!ringbuf_empty(b) ? &(b->data[(b->tail % b->element_count) * b->element_size]) : NULL);
	}
	return NULL;
}

/****************************************************************************
* DESCRIPTION: Looks at the data from the tail of the list without removing it
* RETURN:      pointer to the data, or NULL if nothing in the list
* ALGORITHM:   none
* NOTES:       none
*****************************************************************************/
uint8_t *ringbuf_get_newest(
RING_BUFFER const *b)
{
	if (b) {
		return (!ringbuf_empty(b) ? &(b->data[((b->head-1) % b->element_count) * b->element_size]) : NULL);
	}
	return NULL;
}

/****************************************************************************
* DESCRIPTION: Looks element N of the list without removing it
* RETURN:      pointer to the data, or N is beyond the list
* ALGORITHM:   none
* NOTES:       none
*****************************************************************************/
uint8_t *ringbuf_get_element(RING_BUFFER *b, uint8_t n)
{
	if (b)
	{
		if (ringbuf_count(b) > n)
		{
			return get_element_n(b, n);
		}
	}
	
	return NULL;
}

/****************************************************************************
* DESCRIPTION: Copies N elements from the list to a specified pointer (presumably an array)
* RETURN:      true/false success
* ALGORITHM:   none
* NOTES:       none
*****************************************************************************/
bool ringbuf_get_elements(RING_BUFFER *b, uint8_t startIndex, uint8_t count, RINGBUF_DATA copyBuffer)
{
	bool success = true;
	success &= ((b != NULL) & (copyBuffer != NULL));
	success &= (count > 0);
	
	uint8_t readElementIndex;
	uint8_t copyElementIndex = 0;
	
	if (success)
	{
		success &= (ringbuf_count(b) >= count);
		if (success)
		{
			startIndex = get_element_n_index(b, startIndex);
			readElementIndex = startIndex;
			
			for (uint8_t i = 0; i < count; ++i)
			{
				for (uint8_t byteIndex = 0; byteIndex < b->element_size; byteIndex++)
				{
					copyBuffer[copyElementIndex + byteIndex] = b->data[readElementIndex + byteIndex];
				}
				
				copyElementIndex += b->element_size;
				readElementIndex += b->element_size;
				if (readElementIndex > (b->element_count * b->element_size)) {readElementIndex = 0;}
			}
		}
	}
	
	return success;
}

/****************************************************************************
* DESCRIPTION: Gets the data from the front of the list, and removes it
* RETURN:      pointer to the data, or NULL if nothing in the list
* ALGORITHM:   none
* NOTES:       none
*****************************************************************************/
uint8_t *ringbuf_pop_front(
RING_BUFFER * b)
{
	uint8_t *data = NULL;  /* return value */

	if (!ringbuf_empty(b)) {
		data = get_element_n(b, 0);
		b->tail++;
	}

	return data;
}

/****************************************************************************
* DESCRIPTION: Adds an element of data to the ring buffer
* RETURN:      true on succesful add, false if not added
* ALGORITHM:   none
* NOTES:       none
*****************************************************************************/
bool ringbuf_put(
RING_BUFFER * b,    /* ring buffer structure */
uint8_t *data_element) /* one element to add to the ring */
{
	bool status = false;        /* return value */
	uint8_t *ring_data = NULL;     /* used to help point ring data */
	unsigned i; /* loop counter */

	if (b && data_element) {
		/* limit the amount of elements that we accept */
		if (b && data_element) {
			/* limit the amount of elements that we accept */
			if (b->allowOverwrite && ringbuf_full(b))
			{
				(void)ringbuf_pop_front(b); // Throw away oldest element
			}
			
			if (!ringbuf_full(b) || b->allowOverwrite) {
				ring_data = b->data + ((b->head % b->element_count) * b->element_size);
				for (i = 0; i < b->element_size; i++) {
					ring_data[i] = data_element[i];
				}
				b->head++;
				status = true;
			}
		}
	}

	return status;
}

/****************************************************************************
* DESCRIPTION: Returns the empty/full status of the ring buffer
* RETURN:      true if the ring buffer is empty, false if it is not.
* ALGORITHM:   none
* NOTES:       none
*****************************************************************************/
void ringbuf_allow_overwrite(
RING_BUFFER *b,
bool allow)
{
	if (b) { b->allowOverwrite = allow; }
}

/****************************************************************************
* DESCRIPTION: Converts to a valid power of two
* RETURN:      a valid power of two value within the range of the value given
* ALGORITHM:   none
* NOTES:       none
*****************************************************************************/
static unsigned valid_power_of_two(unsigned k)
{
	unsigned i;

	if ( ((k-1) & k) != 0) {
		k--;
		for (i = 1; i < sizeof(unsigned) * 8; i = i * 2) {
			k = k | k >> i;
		}
		k = (k+1) >> 1;
	}

	return k;
}

/****************************************************************************
* DESCRIPTION: Returns a pointer to a ring buffer element
* RETURN:      Pointer to element. Validity is not checked.
* ALGORITHM:   none
* NOTES:       none
*****************************************************************************/

static uint8_t get_element_n_index(RING_BUFFER *b, uint8_t n)
{
	return ((b->tail + n) % b->element_count) * b->element_size;
}

static uint8_t * get_element_n(RING_BUFFER *b, uint8_t n)
{
	return &(b->data[ get_element_n_index(b, n) ]);
}

/****************************************************************************
* DESCRIPTION: Configures the ring buffer
* RETURN:      none
* ALGORITHM:   none
* NOTES:
*   element_count should be a power of two,
*   or it will only use the next lower power of two
*****************************************************************************/
void ringbuf_init(
RING_BUFFER * b,    /* ring buffer structure */
uint8_t *data, /* data block or array of data */
unsigned element_size,      /* size of one element in the data block */
unsigned element_count,
bool overwriteOnFull)
{       /* number of elements in the data block */
	if (b) {
		/* limit to a power of two - allows a bit to be used to
		count the final slot */
		element_count = valid_power_of_two(element_count);
		b->head = 0;
		b->tail = 0;
		b->data = data;
		b->element_size = element_size;
		b->element_count = element_count;
		b->allowOverwrite = overwriteOnFull;
	}

	return;
}

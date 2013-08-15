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

/****************************************************************************
* DESCRIPTION: Returns the number of elements in the ring buffer
* RETURN:      Number of elements in the ring buffer
* ALGORITHM:   none
* NOTES:       none
*****************************************************************************/
static unsigned Ringbuf_Count (
RING_BUFFER const *b)
{
	return (b ? (b->head - b->tail) : 0);
}

/****************************************************************************
* DESCRIPTION: Returns the empty/full status of the ring buffer
* RETURN:      true if the ring buffer is full, false if it is not.
* ALGORITHM:   none
* NOTES:       none
*****************************************************************************/
static bool Ringbuf_Full (
RING_BUFFER const *b)
{
	return (b ? (Ringbuf_Count(b) == b->element_count) : true);
}

/****************************************************************************
* DESCRIPTION: Returns the empty/full status of the ring buffer
* RETURN:      true if the ring buffer is empty, false if it is not.
* ALGORITHM:   none
* NOTES:       none
*****************************************************************************/
bool Ringbuf_Empty(
RING_BUFFER const *b)
{
	return (b ? (Ringbuf_Count(b) == 0) : true);
}

/****************************************************************************
* DESCRIPTION: Looks at the data from the head of the list without removing it
* RETURN:      pointer to the data, or NULL if nothing in the list
* ALGORITHM:   none
* NOTES:       none
*****************************************************************************/
uint8_t *Ringbuf_Get_Oldest(
RING_BUFFER const *b)
{
	if (b) {
		return (!Ringbuf_Empty(b) ? &(b->data[(b->tail % b->element_count) * b->element_size]) : NULL);
	}
	return NULL;
}

/****************************************************************************
* DESCRIPTION: Looks at the data from the tail of the list without removing it
* RETURN:      pointer to the data, or NULL if nothing in the list
* ALGORITHM:   none
* NOTES:       none
*****************************************************************************/
uint8_t *Ringbuf_Get_Newest(
RING_BUFFER const *b)
{
	if (b) {
		return (!Ringbuf_Empty(b) ? &(b->data[((b->head-1) % b->element_count) * b->element_size]) : NULL);
	}
	return NULL;
}

/****************************************************************************
* DESCRIPTION: Gets the data from the front of the list, and removes it
* RETURN:      pointer to the data, or NULL if nothing in the list
* ALGORITHM:   none
* NOTES:       none
*****************************************************************************/
uint8_t *Ringbuf_Pop_Front(
RING_BUFFER * b)
{
	uint8_t *data = NULL;  /* return value */

	if (!Ringbuf_Empty(b)) {
		data = &(b->data[(b->tail % b->element_count) * b->element_size]);
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
bool Ringbuf_Put(
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
			if (b->allowOverwrite && Ringbuf_Full(b))
			{
				(void)Ringbuf_Pop_Front(b); // Throw away oldest element
			}
			
			if (!Ringbuf_Full(b) || b->allowOverwrite) {
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
void Ringbuf_AllowOverwrite(
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
	int i;

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
* DESCRIPTION: Configures the ring buffer
* RETURN:      none
* ALGORITHM:   none
* NOTES:
*   element_count should be a power of two,
*   or it will only use the next lower power of two
*****************************************************************************/
void Ringbuf_Init(
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

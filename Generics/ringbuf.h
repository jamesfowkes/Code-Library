/*####COPYRIGHTBEGIN####
-------------------------------------------
Copyright (C) 2004 by Steve Karg

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

#ifndef RINGBUF_H
#define RINGBUF_H

#include <stdint.h>
#include <stdbool.h>

typedef uint8_t* RINGBUF_DATA;

struct ring_buffer_t {
	RINGBUF_DATA data; /* block of memory or array of data */
	unsigned element_size;      /* how many bytes for each chunk */
	unsigned element_count;     /* number of chunks of data */
	unsigned head; /* where the writes go */
	unsigned tail; /* where the reads come from */
	bool allowOverwrite;
};
typedef struct ring_buffer_t RING_BUFFER;

#ifdef __cplusplus
 "C" {
#endif /* __cplusplus */

	bool Ringbuf_Full(RING_BUFFER const *b);
	bool Ringbuf_Empty(RING_BUFFER const *b);
	
	unsigned Ringbuf_Count(RING_BUFFER const *b);
	
	RINGBUF_DATA Ringbuf_Get_Oldest(RING_BUFFER const *b);
	RINGBUF_DATA Ringbuf_Get_Newest(RING_BUFFER const *b);
	
	RINGBUF_DATA Ringbuf_Pop_Front(RING_BUFFER * b);
	
	bool Ringbuf_Put(
		RING_BUFFER * b,        /* ring buffer structure */
		RINGBUF_DATA data_element);  /* one element to add to the ring */
	
	void Ringbuf_AllowOverwrite(RING_BUFFER *b,	bool allow);
	
	RINGBUF_DATA Ringbuf_Get_Element(RING_BUFFER *b, uint8_t n);
	bool Ringbuf_Get_Elements(RING_BUFFER *b, uint8_t startIndex, uint8_t count, RINGBUF_DATA copyBuffer);
		
	/* Note: element_count should be a power of two,
	or it will only use the next lower power of two */
	void Ringbuf_Init(
	RING_BUFFER * b,        /* ring buffer structure */
	RINGBUF_DATA data,     /* data block or array of data */
	unsigned element_size,  /* size of one element in the data block */
	unsigned element_count,        /* number of elements in the data block */
	bool overwriteOnFull); /* true if the oldset element should be overwritten if full */
#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif

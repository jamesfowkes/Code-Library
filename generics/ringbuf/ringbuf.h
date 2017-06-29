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

#include "util_macros.h"

uint8_t valid_power_of_two(uint8_t k);

template <typename T>
class RingBuffer
{

private:
	T * m_data; /* block of memory or array of data */
	uint8_t m_max_elements;     /* number of chunks of data */
	uint8_t m_count;
	uint8_t m_head; /* where the writes go */
	uint8_t m_tail; /* where the reads come from */
	bool m_allow_overwrite;

	uint8_t next_read_index();
	uint8_t next_write_index();
public:
	/* Note: element_count should be a power of two,
	or it will only use the next lower power of two */
	RingBuffer(T * data, uint8_t element_count, bool overwriteOnFull);

	bool is_full();
	bool is_empty();

	uint8_t count();

	bool get_oldest(T& data);
	bool get_newest(T& data);

	bool pop_front(T& data);

	bool push_back(T data);

	void allow_overwrite(bool allow);

	bool get_element(T& data, uint8_t n);
	int get_elements(uint8_t startIndex, uint8_t count, T * copyBuffer);
};

template <typename T> 
uint8_t RingBuffer<T>::next_write_index()
{
	return m_head == m_max_elements-1 ? 0 : m_head + 1;
}

template <typename T> 
uint8_t RingBuffer<T>::next_read_index()
{
	return m_tail == m_max_elements-1 ? 0 : m_tail + 1;
}

/****************************************************************************
* DESCRIPTION: Returns the number of elements in the ring buffer
* RETURN:      Number of elements in the ring buffer
* ALGORITHM:   none
* NOTES:       none
*****************************************************************************/
template <typename T> 
uint8_t RingBuffer<T>::count ()
{
	return m_count;
}

/****************************************************************************
* DESCRIPTION: Returns the empty/full status of the ring buffer
* RETURN:      true if the ring buffer is full, false if it is not.
* ALGORITHM:   none
* NOTES:       none
*****************************************************************************/
template <typename T> 
bool RingBuffer<T>::is_full()
{
	return m_count == m_max_elements;
}

/****************************************************************************
* DESCRIPTION: Returns the empty/full status of the ring buffer
* RETURN:      true if the ring buffer is empty, false if it is not.
* ALGORITHM:   none
* NOTES:       none
*****************************************************************************/
template <typename T> 
bool RingBuffer<T>::is_empty()
{
	return m_count == 0;
}

/****************************************************************************
* DESCRIPTION: Looks at the data from the head of the list without removing it
* RETURN:      true if reference was updated with oldest value, false otherwise
* ALGORITHM:   none
* NOTES:       none
*****************************************************************************/
template <typename T> 
bool RingBuffer<T>::get_oldest(T& data)
{
	if (is_empty()) return false;

	data = m_data[m_tail];
	
	return true;
}

/****************************************************************************
* DESCRIPTION: Looks at the data from the tail of the list without removing it
* RETURN:      true if reference was updated with newest value, false otherwise
* ALGORITHM:   none
* NOTES:       none
*****************************************************************************/
template <typename T> 
bool RingBuffer<T>::get_newest(T& data)
{
	if (is_empty()) return false;

	data = m_data[m_head-1];
	
	return true;
}

/****************************************************************************
* DESCRIPTION: Looks element N of the list without removing it
* RETURN:      pointer to the data, or N is beyond the list
* ALGORITHM:   none
* NOTES:       none
*****************************************************************************/
template <typename T> 
bool RingBuffer<T>::get_element(T& data, uint8_t n)
{
	if (n > m_count) { return false; }
	if (n > m_tail) { n -= m_tail; }
	data = m_data[m_tail+n];
	return true;
}

/****************************************************************************
* DESCRIPTION: Copies N elements from the list to a specified pointer (presumably an array)
* RETURN:      number of elements copied
* ALGORITHM:   none
* NOTES:       none
*****************************************************************************/
template <typename T> 
int RingBuffer<T>::get_elements(uint8_t startIndex, uint8_t count, T * copyBuffer)
{
	uint8_t i;
	
	uint8_t copy_count = 0;
	uint8_t index = m_tail+startIndex;

	index = index % m_max_elements;

	if ((copyBuffer != NULL) && (count > 0))
	{
		for (i = 0; i < count; ++i)
		{
			copyBuffer[i] = m_data[index];
			copy_count++;
			incrementwithrollover(index, m_max_elements-1);
		}
	}
	
	return copy_count;
}

/****************************************************************************
* DESCRIPTION: Gets the data from the front of the list, and removes it
* RETURN:      pointer to the data, or NULL if nothing in the list
* ALGORITHM:   none
* NOTES:       none
*****************************************************************************/
template <typename T> 
bool RingBuffer<T>::pop_front(T& data)
{
	if (m_count == 0) { return false; }
	memcpy(data, m_data[m_tail], sizeof(T));
	m_tail = next_read_index();
	m_count--;
	
	return true;
}

/****************************************************************************
* DESCRIPTION: Adds an element of data to the ring buffer
* RETURN:      true on succesful add, false if not added
* ALGORITHM:   none
* NOTES:       none
*****************************************************************************/
template <typename T> 
bool RingBuffer<T>::push_back(T data)
{
	bool status = false;        /* return value */
	T throwaway;

	/* limit the amount of elements that we accept */
	if (m_allow_overwrite && is_full())
	{
		(void)pop_front(throwaway); // Throw away oldest element
	}
	
	if (!is_full() || m_allow_overwrite) {
		memcpy(m_data[m_tail], data, sizeof(T));
		m_head = next_write_index();
		m_count++;

		status = true;
	}


	return status;
}

/****************************************************************************
* DESCRIPTION: Returns the empty/full status of the ring buffer
* RETURN:      true if the ring buffer is empty, false if it is not.
* ALGORITHM:   none
* NOTES:       none
*****************************************************************************/
template <typename T> 
void RingBuffer<T>::allow_overwrite(bool allow)
{
	m_allow_overwrite = allow;
}

/****************************************************************************
* DESCRIPTION: Configures the ring buffer
* RETURN:      none
* ALGORITHM:   none
* NOTES:
*   element_count should be a power of two,
*   or it will only use the next lower power of two
*****************************************************************************/
template <typename T> 
RingBuffer<T>::RingBuffer(T *data, uint8_t element_count, bool overwriteOnFull)
{
	/* limit to a power of two - allows a bit to be used to
	count the final slot */
	element_count = valid_power_of_two(element_count);
	m_head = 0;
	m_tail = 0;
	m_count = 0;
	m_data = data;
	m_max_elements = element_count;
	m_allow_overwrite = overwriteOnFull;

	return;
}

#endif

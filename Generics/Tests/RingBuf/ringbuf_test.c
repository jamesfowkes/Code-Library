#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

#include "unity.h"

#include "../../ringbuf.h"
#include "util_macros.h"

static RING_BUFFER buffer;
static uint16_t data[16];

static uint16_t scratchpad;

void setUp(void)
{
	Ringbuf_Init(&buffer, (RINGBUF_DATA)data, sizeof(data[0]), 16, true);
}

void tearDown(void)
{

}

void test_RingBufInit(void)
{
	TEST_ASSERT_EQUAL(0, buffer.head);
	TEST_ASSERT_EQUAL(0, buffer.tail);
	TEST_ASSERT_EQUAL(data, buffer.data);
	TEST_ASSERT_EQUAL(sizeof(data[0]), buffer.element_size);
	TEST_ASSERT_EQUAL(16, buffer.element_count);
	TEST_ASSERT_EQUAL(true, buffer.allowOverwrite);
	TEST_ASSERT_EQUAL(Ringbuf_Count(&buffer), 0);
	TEST_ASSERT_EQUAL(true, Ringbuf_Empty(&buffer));
	TEST_ASSERT_EQUAL(false, Ringbuf_Full(&buffer));
}

void test_AddOneItem(void)
{
	scratchpad = 123;
	Ringbuf_Put(&buffer, (RINGBUF_DATA)&scratchpad);
	TEST_ASSERT_EQUAL(123, data[0]);
	TEST_ASSERT_EQUAL(Ringbuf_Count(&buffer), 1);
	TEST_ASSERT_EQUAL(false, Ringbuf_Empty(&buffer));
	TEST_ASSERT_EQUAL(false, Ringbuf_Full(&buffer));
}

void test_MultipleItems(void)
{
	uint8_t i;
	for (i = 0; i < 5; i++)
	{
		scratchpad = i;
		Ringbuf_Put(&buffer, (RINGBUF_DATA)&scratchpad);
		TEST_ASSERT_EQUAL(i, data[i]);
		TEST_ASSERT_EQUAL(i + 1, Ringbuf_Count(&buffer));
		TEST_ASSERT_EQUAL(false, Ringbuf_Empty(&buffer));
		TEST_ASSERT_EQUAL(false, Ringbuf_Full(&buffer));
	}
}

void test_AddToFull(void)
{
	uint8_t i;
	for (i = 0; i < 16; i++)
	{
		scratchpad = i;
		Ringbuf_Put(&buffer, (RINGBUF_DATA)&scratchpad);
		TEST_ASSERT_EQUAL(i, data[i]);
		TEST_ASSERT_EQUAL(i + 1, Ringbuf_Count(&buffer));
		TEST_ASSERT_EQUAL(false, Ringbuf_Empty(&buffer));
		TEST_ASSERT_EQUAL(i == 15, Ringbuf_Full(&buffer));
	}
}

void test_AddOverFull(void)
{
	uint8_t i;
	uint8_t dataIndex = 0;
	
	for (i = 0; i < 24; i++)
	{
		scratchpad = i;
		Ringbuf_Put(&buffer, (RINGBUF_DATA)&scratchpad);
		TEST_ASSERT_EQUAL(i, data[dataIndex]);
		
		incrementwithrollover(dataIndex, 15);
		
		TEST_ASSERT_EQUAL(Ringbuf_Count(&buffer), min(i+1, 16));
		TEST_ASSERT_EQUAL(false, Ringbuf_Empty(&buffer));
		TEST_ASSERT_EQUAL(i >= 15, Ringbuf_Full(&buffer));
		
	}
}

void test_GetItems(void)
{
	uint8_t i;
	for (i = 0; i < 24; i++)
	{
		scratchpad = i;
		Ringbuf_Put(&buffer, (RINGBUF_DATA)&scratchpad);
	}
	
	TEST_ASSERT_EQUAL(8, *(uint16_t*)Ringbuf_Get_Oldest(&buffer));
	TEST_ASSERT_EQUAL(23, *(uint16_t*)Ringbuf_Get_Newest(&buffer));
	
	TEST_ASSERT_EQUAL(12, *(uint16_t*)Ringbuf_Get_Element(&buffer, 4));
	
	TEST_ASSERT_EQUAL(NULL, (uint16_t*)Ringbuf_Get_Element(&buffer, 17));
}

void test_PopItems(void)
{
	uint8_t i;
	for (i = 0; i < 16; i++)
	{
		scratchpad = i;
		Ringbuf_Put(&buffer, (RINGBUF_DATA)&scratchpad);
	}
	
	for (i = 0; i < 16; i++)
	{
		TEST_ASSERT_EQUAL(16-i, Ringbuf_Count(&buffer));
		TEST_ASSERT_EQUAL(i, *(uint16_t*)Ringbuf_Pop_Front(&buffer));
	}
}

void test_GetMultipleItems(void)
{
	uint8_t i;
	for (i = 0; i < 24; i++)
	{
		scratchpad = i;
		Ringbuf_Put(&buffer, (RINGBUF_DATA)&scratchpad);
	}

	uint16_t copyBuf[5] = {0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF};
	
	Ringbuf_Get_Elements(&buffer, 2, 4, (RINGBUF_DATA)copyBuf);
	
	TEST_ASSERT_EQUAL(10, copyBuf[0]);
	TEST_ASSERT_EQUAL(11, copyBuf[1]);
	TEST_ASSERT_EQUAL(12, copyBuf[2]);
	TEST_ASSERT_EQUAL(13, copyBuf[3]);
	TEST_ASSERT_EQUAL(0xFFFF, copyBuf[4]);
	
	Ringbuf_Get_Elements(&buffer, 14, 4, (RINGBUF_DATA)copyBuf);
	
	TEST_ASSERT_EQUAL(22, copyBuf[0]);
	TEST_ASSERT_EQUAL(23, copyBuf[1]);
	TEST_ASSERT_EQUAL(8, copyBuf[2]);
	TEST_ASSERT_EQUAL(9, copyBuf[3]);
	TEST_ASSERT_EQUAL(0xFFFF, copyBuf[4]);
	
	
}

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/extensions/HelperMacros.h>

#include "ringbuf.h"
#include "util_macros.h"

static RING_BUFFER buffer;
static uint16_t data[16];

static uint16_t scratchpad;

class RingBufTest : public CppUnit::TestFixture  {

	CPPUNIT_TEST_SUITE(RingBufTest);

	CPPUNIT_TEST(test_ringbufinit);
	CPPUNIT_TEST(test_addoneitem);
	CPPUNIT_TEST(test_multipleitems);
	CPPUNIT_TEST(test_addtofull);
	CPPUNIT_TEST(test_addoverfull);
	CPPUNIT_TEST(test_getitems);
	CPPUNIT_TEST(test_popitems);
	CPPUNIT_TEST(test_getmultipleitems);

	CPPUNIT_TEST_SUITE_END();

	void test_ringbufinit(void)
	{
		CPPUNIT_ASSERT_EQUAL(0U, buffer.head);
		CPPUNIT_ASSERT_EQUAL(0U, buffer.tail);
		CPPUNIT_ASSERT_EQUAL((uint16_t*)data, (uint16_t*)buffer.data);
		CPPUNIT_ASSERT_EQUAL(sizeof(data[0]), (size_t)buffer.element_size);
		CPPUNIT_ASSERT_EQUAL(16U, buffer.element_count);
		CPPUNIT_ASSERT_EQUAL(true, buffer.allowOverwrite);
		CPPUNIT_ASSERT_EQUAL(0U, ringbuf_count(&buffer));
		CPPUNIT_ASSERT_EQUAL(true, ringbuf_empty(&buffer));
		CPPUNIT_ASSERT_EQUAL(false, ringbuf_full(&buffer));
	}

	void test_addoneitem(void)
	{
		scratchpad = 123;
		ringbuf_put(&buffer, (RINGBUF_DATA)&scratchpad);
		CPPUNIT_ASSERT_EQUAL((uint16_t)123, data[0]);
		CPPUNIT_ASSERT_EQUAL(1U, ringbuf_count(&buffer));
		CPPUNIT_ASSERT_EQUAL(false, ringbuf_empty(&buffer));
		CPPUNIT_ASSERT_EQUAL(false, ringbuf_full(&buffer));
	}

	void test_multipleitems(void)
	{
		uint16_t i;
		for (i = 0; i < 5; i++)
		{
			scratchpad = i;
			ringbuf_put(&buffer, (RINGBUF_DATA)&scratchpad);
			CPPUNIT_ASSERT_EQUAL(i, data[i]);
			CPPUNIT_ASSERT_EQUAL(i + 1U, ringbuf_count(&buffer));
			CPPUNIT_ASSERT_EQUAL(false, ringbuf_empty(&buffer));
			CPPUNIT_ASSERT_EQUAL(false, ringbuf_full(&buffer));
		}
	}

	void test_addtofull(void)
	{
		uint16_t i;
		for (i = 0; i < 16; i++)
		{
			scratchpad = i;
			ringbuf_put(&buffer, (RINGBUF_DATA)&scratchpad);
			CPPUNIT_ASSERT_EQUAL(i, data[i]);
			CPPUNIT_ASSERT_EQUAL(i + 1U, ringbuf_count(&buffer));
			CPPUNIT_ASSERT_EQUAL(false, ringbuf_empty(&buffer));
			CPPUNIT_ASSERT_EQUAL(i == 15, ringbuf_full(&buffer));
		}
	}

	void test_addoverfull(void)
	{
		uint16_t i;
		uint8_t dataIndex = 0;
		
		for (i = 0; i < 24; i++)
		{
			scratchpad = i;
			ringbuf_put(&buffer, (RINGBUF_DATA)&scratchpad);
			CPPUNIT_ASSERT_EQUAL(i, data[dataIndex]);
			
			incrementwithrollover(dataIndex, 15);
			
			CPPUNIT_ASSERT_EQUAL(ringbuf_count(&buffer), min(i+1U, 16U));
			CPPUNIT_ASSERT_EQUAL(false, ringbuf_empty(&buffer));
			CPPUNIT_ASSERT_EQUAL(i >= 15, ringbuf_full(&buffer));
			
		}
	}

	void test_getitems(void)
	{
		uint8_t i;
		for (i = 0; i < 24; i++)
		{
			scratchpad = i;
			ringbuf_put(&buffer, (RINGBUF_DATA)&scratchpad);
		}
		
		CPPUNIT_ASSERT_EQUAL((uint16_t)8, *(uint16_t*)ringbuf_get_oldest(&buffer));
		CPPUNIT_ASSERT_EQUAL((uint16_t)23, *(uint16_t*)ringbuf_get_newest(&buffer));
		
		CPPUNIT_ASSERT_EQUAL((uint16_t)12, *(uint16_t*)ringbuf_get_element(&buffer, 4));
		
		CPPUNIT_ASSERT(NULL == (uint16_t*)ringbuf_get_element(&buffer, 17));
	}

	void test_popitems(void)
	{
		uint16_t i;
		for (i = 0; i < 16; i++)
		{
			scratchpad = i;
			ringbuf_put(&buffer, (RINGBUF_DATA)&scratchpad);
		}
		
		for (i = 0; i < 16; i++)
		{
			CPPUNIT_ASSERT_EQUAL(16U-i, ringbuf_count(&buffer));
			CPPUNIT_ASSERT_EQUAL(i, *(uint16_t*)ringbuf_pop_front(&buffer));
		}
	}

	void test_getmultipleitems(void)
	{
		uint8_t i;
		for (i = 0; i < 24; i++)
		{
			scratchpad = i;
			ringbuf_put(&buffer, (RINGBUF_DATA)&scratchpad);
		}

		uint16_t copyBuf[5] = {0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF};
		
		ringbuf_get_elements(&buffer, 2, 4, (RINGBUF_DATA)copyBuf);
		
		CPPUNIT_ASSERT_EQUAL((uint16_t)10, copyBuf[0]);
		CPPUNIT_ASSERT_EQUAL((uint16_t)11, copyBuf[1]);
		CPPUNIT_ASSERT_EQUAL((uint16_t)12, copyBuf[2]);
		CPPUNIT_ASSERT_EQUAL((uint16_t)13, copyBuf[3]);
		CPPUNIT_ASSERT_EQUAL((uint16_t)0xFFFF, copyBuf[4]);
		
		ringbuf_get_elements(&buffer, 14, 4, (RINGBUF_DATA)copyBuf);
		
		CPPUNIT_ASSERT_EQUAL((uint16_t)22, copyBuf[0]);
		CPPUNIT_ASSERT_EQUAL((uint16_t)23, copyBuf[1]);
		CPPUNIT_ASSERT_EQUAL((uint16_t)8, copyBuf[2]);
		CPPUNIT_ASSERT_EQUAL((uint16_t)9, copyBuf[3]);
		CPPUNIT_ASSERT_EQUAL((uint16_t)0xFFFF, copyBuf[4]);
		
		
	}

public:
	void setUp(void)
	{
		ringbuf_init(&buffer, (RINGBUF_DATA)data, sizeof(data[0]), 16, true);
	}

	void tearDown(void)
	{

	}
};

int main()
{
   CppUnit::TextUi::TestRunner runner;
   
   CPPUNIT_TEST_SUITE_REGISTRATION( RingBufTest );

   CppUnit::TestFactoryRegistry &registry = CppUnit::TestFactoryRegistry::getRegistry();

   runner.addTest( registry.makeTest() );
   runner.run();

   return 0;
}

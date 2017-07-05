#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/extensions/HelperMacros.h>

#include "ringbuf.h"
#include "util_macros.h"

static uint16_t data[16];
static RingBuffer<uint16_t> * buffer;

static uint16_t scratchpad;

class RingBufferTest : public CppUnit::TestFixture  {

	CPPUNIT_TEST_SUITE(RingBufferTest);

	CPPUNIT_TEST(test_ringbufinit);
	CPPUNIT_TEST(test_addoneitem);
	CPPUNIT_TEST(test_multipleitems);
	CPPUNIT_TEST(test_addtofull);
	CPPUNIT_TEST(test_addoverfull);
	CPPUNIT_TEST(test_getitems);
	CPPUNIT_TEST(test_popitems);
	CPPUNIT_TEST(test_getmultipleitems);
	CPPUNIT_TEST(test_pop_with_no_elements);

	CPPUNIT_TEST_SUITE_END();

	void test_ringbufinit(void)
	{
		CPPUNIT_ASSERT_EQUAL((uint8_t)0, buffer->count());
		CPPUNIT_ASSERT_EQUAL(true, buffer->is_empty());
		CPPUNIT_ASSERT_EQUAL(false, buffer->is_full());
	}

	void test_addoneitem(void)
	{
		scratchpad = 123;
		CPPUNIT_ASSERT(buffer->push_back(scratchpad));
		CPPUNIT_ASSERT_EQUAL((uint16_t)123, data[0]);
		CPPUNIT_ASSERT_EQUAL((uint8_t)1, buffer->count());
		CPPUNIT_ASSERT_EQUAL(false, buffer->is_empty());
		CPPUNIT_ASSERT_EQUAL(false, buffer->is_full());
	}

	void test_multipleitems(void)
	{
		uint8_t i;
		for (i = 0; i < 5; i++)
		{
			scratchpad = (uint16_t)i;
			CPPUNIT_ASSERT(buffer->push_back(scratchpad));
			CPPUNIT_ASSERT_EQUAL((uint16_t)i, data[i]);
			CPPUNIT_ASSERT_EQUAL((uint8_t)(i + 1U), buffer->count());
			CPPUNIT_ASSERT_EQUAL(false, buffer->is_empty());
			CPPUNIT_ASSERT_EQUAL(false, buffer->is_full());
		}
	}

	void test_addtofull(void)
	{
		uint16_t i;
		for (i = 0; i < 16; i++)
		{
			scratchpad = i;
			CPPUNIT_ASSERT(buffer->push_back(scratchpad));
			CPPUNIT_ASSERT_EQUAL(i, data[i]);
			CPPUNIT_ASSERT_EQUAL((uint8_t)(i + 1U), buffer->count());
			CPPUNIT_ASSERT_EQUAL(false, buffer->is_empty());
			CPPUNIT_ASSERT_EQUAL(i == 15, buffer->is_full());
		}
	}

	void test_addoverfull(void)
	{
		uint16_t i;
		uint8_t dataIndex = 0;
		
		for (i = 0; i < 24; i++)
		{
			scratchpad = i;
			CPPUNIT_ASSERT(buffer->push_back(scratchpad));
			CPPUNIT_ASSERT_EQUAL(i, data[dataIndex]);
			
			incrementwithrollover(dataIndex, 15);
			
			CPPUNIT_ASSERT_EQUAL((uint8_t)min(i+1U, 16U), buffer->count());
			CPPUNIT_ASSERT_EQUAL(false, buffer->is_empty());
			CPPUNIT_ASSERT_EQUAL(i >= 15, buffer->is_full());
			
		}
	}

	void test_getitems(void)
	{
		uint8_t i;
		uint16_t result;
		for (i = 0; i < 24; i++)
		{
			scratchpad = i;
			CPPUNIT_ASSERT(buffer->push_back(scratchpad));
		}
		
		CPPUNIT_ASSERT_EQUAL((uint8_t)16, buffer->count());

		CPPUNIT_ASSERT(buffer->get_oldest(result));
		CPPUNIT_ASSERT_EQUAL((uint16_t)8, result);

		CPPUNIT_ASSERT(buffer->get_newest(result));
		CPPUNIT_ASSERT_EQUAL((uint16_t)23, result);
		
		CPPUNIT_ASSERT(buffer->get_element(result, 4));
		CPPUNIT_ASSERT_EQUAL((uint16_t)12, result);
		
		CPPUNIT_ASSERT(!buffer->get_element(result, 17));
	}

	void test_popitems(void)
	{
		uint8_t i;
		uint16_t result;
		for (i = 0; i < 16; i++)
		{
			scratchpad = (uint16_t)i;
			CPPUNIT_ASSERT(buffer->push_back(scratchpad));
		}
		
		for (i = 0; i < 16; i++)
		{
			CPPUNIT_ASSERT_EQUAL((uint8_t)(16-i), buffer->count());
			CPPUNIT_ASSERT(buffer->pop_front(result));
			CPPUNIT_ASSERT_EQUAL((uint16_t)i, result);
		}
	}

	void test_getmultipleitems(void)
	{
		uint8_t i;
		for (i = 0; i < 24; i++)
		{
			scratchpad = i;
			CPPUNIT_ASSERT(buffer->push_back(scratchpad));
		}

		uint16_t copyBuf[5] = {0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF};
		
		CPPUNIT_ASSERT_EQUAL(4, buffer->get_elements(2, 4, copyBuf));
		
		CPPUNIT_ASSERT_EQUAL((uint16_t)10, copyBuf[0]);
		CPPUNIT_ASSERT_EQUAL((uint16_t)11, copyBuf[1]);
		CPPUNIT_ASSERT_EQUAL((uint16_t)12, copyBuf[2]);
		CPPUNIT_ASSERT_EQUAL((uint16_t)13, copyBuf[3]);
		CPPUNIT_ASSERT_EQUAL((uint16_t)0xFFFF, copyBuf[4]);
		
		CPPUNIT_ASSERT_EQUAL(4, buffer->get_elements(14, 4, copyBuf));
		
		CPPUNIT_ASSERT_EQUAL((uint16_t)22, copyBuf[0]);
		CPPUNIT_ASSERT_EQUAL((uint16_t)23, copyBuf[1]);
		CPPUNIT_ASSERT_EQUAL((uint16_t)8, copyBuf[2]);
		CPPUNIT_ASSERT_EQUAL((uint16_t)9, copyBuf[3]);
		CPPUNIT_ASSERT_EQUAL((uint16_t)0xFFFF, copyBuf[4]);
	}

	void test_pop_with_no_elements(void)
	{
		scratchpad = 123;
		CPPUNIT_ASSERT(buffer->push_back(scratchpad++));
		CPPUNIT_ASSERT(buffer->push_back(scratchpad++));
		CPPUNIT_ASSERT(buffer->push_back(scratchpad++));

		CPPUNIT_ASSERT(buffer->pop_front(scratchpad));
		CPPUNIT_ASSERT_EQUAL((uint16_t)123, scratchpad);
	
		CPPUNIT_ASSERT(buffer->pop_front(scratchpad));
		CPPUNIT_ASSERT_EQUAL((uint16_t)124, scratchpad);
	
		CPPUNIT_ASSERT(buffer->pop_front(scratchpad));
		CPPUNIT_ASSERT_EQUAL((uint16_t)125, scratchpad);

		CPPUNIT_ASSERT(!buffer->pop_front(scratchpad));
		CPPUNIT_ASSERT_EQUAL((uint16_t)125, scratchpad);
	}
	

public:
	
	void setUp()
	{
		buffer = new RingBuffer<uint16_t>(data, 16, true);
	}

	void tearDown()
	{
		delete buffer;
	}

};

int main()
{
   CppUnit::TextUi::TestRunner runner;
   
   CPPUNIT_TEST_SUITE_REGISTRATION( RingBufferTest );

   CppUnit::TestFactoryRegistry &registry = CppUnit::TestFactoryRegistry::getRegistry();

   runner.addTest( registry.makeTest() );
   runner.run();

   return 0;
}

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

static uint16_t int_data[16];
static uint16_t scratchpad;
static RingBuffer<uint16_t> * int_buffer;

static char string_data[16][32];
static char string_scratchpad[32];

static RingBuffer<char[32]> * string_buffer;

class RingBufferTest : public CppUnit::TestFixture  {

	CPPUNIT_TEST_SUITE(RingBufferTest);

	CPPUNIT_TEST(test_int_ringbufinit);
	CPPUNIT_TEST(test_int_addoneitem);
	CPPUNIT_TEST(test_int_multipleitems);
	CPPUNIT_TEST(test_int_addtofull);
	CPPUNIT_TEST(test_int_addoverfull);
	CPPUNIT_TEST(test_int_getitems);
	CPPUNIT_TEST(test_int_popitems);
	CPPUNIT_TEST(test_int_getmultipleitems);
	CPPUNIT_TEST(test_int_pop_with_no_elements);

	CPPUNIT_TEST(test_string_addoneitem);
	CPPUNIT_TEST(test_string_getitems);

	CPPUNIT_TEST_SUITE_END();

	void test_int_ringbufinit(void)
	{
		CPPUNIT_ASSERT_EQUAL((uint8_t)0, int_buffer->count());
		CPPUNIT_ASSERT_EQUAL(true, int_buffer->is_empty());
		CPPUNIT_ASSERT_EQUAL(false, int_buffer->is_full());
	}

	void test_int_addoneitem(void)
	{
		scratchpad = 123;
		CPPUNIT_ASSERT(int_buffer->push_back(scratchpad));
		CPPUNIT_ASSERT_EQUAL((uint16_t)123, int_data[0]);
		CPPUNIT_ASSERT_EQUAL((uint8_t)1, int_buffer->count());
		CPPUNIT_ASSERT_EQUAL(false, int_buffer->is_empty());
		CPPUNIT_ASSERT_EQUAL(false, int_buffer->is_full());
	}

	void test_int_multipleitems(void)
	{
		uint8_t i;
		for (i = 0; i < 5; i++)
		{
			scratchpad = (uint16_t)i;
			CPPUNIT_ASSERT(int_buffer->push_back(scratchpad));
			CPPUNIT_ASSERT_EQUAL((uint16_t)i, int_data[i]);
			CPPUNIT_ASSERT_EQUAL((uint8_t)(i + 1U), int_buffer->count());
			CPPUNIT_ASSERT_EQUAL(false, int_buffer->is_empty());
			CPPUNIT_ASSERT_EQUAL(false, int_buffer->is_full());
		}
	}

	void test_int_addtofull(void)
	{
		uint16_t i;
		for (i = 0; i < 16; i++)
		{
			scratchpad = i;
			CPPUNIT_ASSERT(int_buffer->push_back(scratchpad));
			CPPUNIT_ASSERT_EQUAL(i, int_data[i]);
			CPPUNIT_ASSERT_EQUAL((uint8_t)(i + 1U), int_buffer->count());
			CPPUNIT_ASSERT_EQUAL(false, int_buffer->is_empty());
			CPPUNIT_ASSERT_EQUAL(i == 15, int_buffer->is_full());
		}
	}

	void test_int_addoverfull(void)
	{
		uint16_t i;
		uint8_t data_index = 0;
		
		for (i = 0; i < 24; i++)
		{
			scratchpad = i;
			CPPUNIT_ASSERT(int_buffer->push_back(scratchpad));
			CPPUNIT_ASSERT_EQUAL(i, int_data[data_index]);
			
			incrementwithrollover(data_index, 15);
			
			CPPUNIT_ASSERT_EQUAL((uint8_t)min(i+1U, 16U), int_buffer->count());
			CPPUNIT_ASSERT_EQUAL(false, int_buffer->is_empty());
			CPPUNIT_ASSERT_EQUAL(i >= 15, int_buffer->is_full());
			
		}
	}

	void test_int_getitems(void)
	{
		uint8_t i;
		uint16_t result;
		for (i = 0; i < 24; i++)
		{
			scratchpad = i;
			CPPUNIT_ASSERT(int_buffer->push_back(scratchpad));
		}
		
		CPPUNIT_ASSERT_EQUAL((uint8_t)16, int_buffer->count());

		CPPUNIT_ASSERT(int_buffer->get_oldest(result));
		CPPUNIT_ASSERT_EQUAL((uint16_t)8, result);

		CPPUNIT_ASSERT(int_buffer->get_newest(result));
		CPPUNIT_ASSERT_EQUAL((uint16_t)23, result);
		
		CPPUNIT_ASSERT(int_buffer->get_element(result, 4));
		CPPUNIT_ASSERT_EQUAL((uint16_t)12, result);
		
		CPPUNIT_ASSERT(!int_buffer->get_element(result, 17));
	}

	void test_int_popitems(void)
	{
		uint8_t i;
		uint16_t result;
		for (i = 0; i < 16; i++)
		{
			scratchpad = (uint16_t)i;
			CPPUNIT_ASSERT(int_buffer->push_back(scratchpad));
		}
		
		for (i = 0; i < 16; i++)
		{
			CPPUNIT_ASSERT_EQUAL((uint8_t)(16-i), int_buffer->count());
			CPPUNIT_ASSERT(int_buffer->pop_front(result));
			CPPUNIT_ASSERT_EQUAL((uint16_t)i, result);
		}
	}

	void test_int_getmultipleitems(void)
	{
		uint8_t i;
		for (i = 0; i < 24; i++)
		{
			scratchpad = i;
			CPPUNIT_ASSERT(int_buffer->push_back(scratchpad));
		}

		uint16_t copyBuf[5] = {0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF};
		
		CPPUNIT_ASSERT_EQUAL(4, int_buffer->get_elements(2, 4, copyBuf));
		
		CPPUNIT_ASSERT_EQUAL((uint16_t)10, copyBuf[0]);
		CPPUNIT_ASSERT_EQUAL((uint16_t)11, copyBuf[1]);
		CPPUNIT_ASSERT_EQUAL((uint16_t)12, copyBuf[2]);
		CPPUNIT_ASSERT_EQUAL((uint16_t)13, copyBuf[3]);
		CPPUNIT_ASSERT_EQUAL((uint16_t)0xFFFF, copyBuf[4]);
		
		CPPUNIT_ASSERT_EQUAL(4, int_buffer->get_elements(14, 4, copyBuf));
		
		CPPUNIT_ASSERT_EQUAL((uint16_t)22, copyBuf[0]);
		CPPUNIT_ASSERT_EQUAL((uint16_t)23, copyBuf[1]);
		CPPUNIT_ASSERT_EQUAL((uint16_t)8, copyBuf[2]);
		CPPUNIT_ASSERT_EQUAL((uint16_t)9, copyBuf[3]);
		CPPUNIT_ASSERT_EQUAL((uint16_t)0xFFFF, copyBuf[4]);
	}

	void test_int_pop_with_no_elements(void)
	{
		scratchpad = 123;
		CPPUNIT_ASSERT(int_buffer->push_back(scratchpad++));
		CPPUNIT_ASSERT(int_buffer->push_back(scratchpad++));
		CPPUNIT_ASSERT(int_buffer->push_back(scratchpad++));

		CPPUNIT_ASSERT(int_buffer->pop_front(scratchpad));
		CPPUNIT_ASSERT_EQUAL((uint16_t)123, scratchpad);
	
		CPPUNIT_ASSERT(int_buffer->pop_front(scratchpad));
		CPPUNIT_ASSERT_EQUAL((uint16_t)124, scratchpad);
	
		CPPUNIT_ASSERT(int_buffer->pop_front(scratchpad));
		CPPUNIT_ASSERT_EQUAL((uint16_t)125, scratchpad);

		CPPUNIT_ASSERT(!int_buffer->pop_front(scratchpad));
		CPPUNIT_ASSERT_EQUAL((uint16_t)125, scratchpad);
	}
	
	void test_string_addoneitem(void)
	{
		strcpy(string_scratchpad, "ABCDEFGHIJKLMnopqrstuvwxyz");

		CPPUNIT_ASSERT(string_buffer->push_back(string_scratchpad));
		CPPUNIT_ASSERT_EQUAL(strcmp(string_scratchpad, string_data[0]), 0);
		CPPUNIT_ASSERT_EQUAL((uint8_t)1, string_buffer->count());
		CPPUNIT_ASSERT_EQUAL(false, string_buffer->is_empty());
		CPPUNIT_ASSERT_EQUAL(false, string_buffer->is_full());
	}

	void test_string_getitems(void)
	{
		uint8_t i;
		char result[32];
		for (i = 0; i < 24; i++)
		{
			sprintf(string_scratchpad, "%d*%d=%d",i,i,i*i);
			CPPUNIT_ASSERT(string_buffer->push_back(string_scratchpad));
		}
		
		CPPUNIT_ASSERT_EQUAL((uint8_t)16, string_buffer->count());

		CPPUNIT_ASSERT(string_buffer->get_oldest(result));
		CPPUNIT_ASSERT_EQUAL(strcmp("8*8=64", result), 0);

		CPPUNIT_ASSERT(string_buffer->get_newest(result));
		CPPUNIT_ASSERT_EQUAL(strcmp("23*23=529", result), 0);
		
		CPPUNIT_ASSERT(string_buffer->get_element(result, 4));
		CPPUNIT_ASSERT_EQUAL(strcmp("12*12=144", result), 0);
		
		CPPUNIT_ASSERT(!string_buffer->get_element(result, 17));
	}

public:
	
	void setUp()
	{
		int_buffer = new RingBuffer<uint16_t>(int_data, 16, true);
		string_buffer = new RingBuffer<char[32]>(string_data, 16, true);
	}

	void tearDown()
	{
		delete int_buffer;
		delete string_buffer;
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

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

#include "unity.h"

#include "../../memorypool.h"

struct good_test
{
	uint8_t bytes[MEMORY_POOL_BYTES - 1];
};
typedef struct good_test GOOD_TEST;

struct exact_test
{
	uint8_t bytes[MEMORY_POOL_BYTES];
};
typedef struct exact_test EXACT_TEST;

struct bad_test
{
	uint8_t bytes[MEMORY_POOL_BYTES + 1];
};
typedef struct bad_test BAD_TEST;

void setUp(void)
{

}

void tearDown(void)
{
	MEMPOOL_Reset();
}

void test_ZeroReturnsNull(void)
{
	printf("Testing with 0 bytes\n");
	TEST_ASSERT_NULL(MEMPOOL_GetBytes(0));
	TEST_ASSERT_EQUAL(MEMORY_POOL_BYTES, MEMPOOL_GetRemaining());
	TEST_ASSERT_EQUAL(0, MEMPOOL_GetUsed());
}

void test_GetWithinBounds(void)
{
	printf("Testing with %d size struct\n", sizeof(GOOD_TEST));
	TEST_ASSERT_NOT_NULL(MEMPOOL_GetBytes(sizeof(GOOD_TEST)));
	TEST_ASSERT_EQUAL(1, MEMPOOL_GetRemaining());
	TEST_ASSERT_EQUAL(MEMORY_POOL_BYTES-1, MEMPOOL_GetUsed());
}

void test_GetOutsideBounds(void)
{
	printf("Testing with %d size struct\n", sizeof(BAD_TEST));
	TEST_ASSERT_NULL(MEMPOOL_GetBytes(sizeof(BAD_TEST)));
	TEST_ASSERT_EQUAL(MEMORY_POOL_BYTES, MEMPOOL_GetRemaining());
	TEST_ASSERT_EQUAL(0, MEMPOOL_GetUsed());
}

void test_GetExactBounds(void)
{
	printf("Testing with %d size struct\n", sizeof(EXACT_TEST));
	TEST_ASSERT_NOT_NULL(MEMPOOL_GetBytes(sizeof(EXACT_TEST)));
	TEST_ASSERT_EQUAL(0, MEMPOOL_GetRemaining());
	TEST_ASSERT_EQUAL(MEMORY_POOL_BYTES, MEMPOOL_GetUsed());
}

void test_GetOneByteAtATime(void)
{
	uint16_t i = 0;
	
	for (i = 0; i < MEMORY_POOL_BYTES; ++i)
	{
		TEST_ASSERT_NOT_NULL(MEMPOOL_GetBytes(1));
		TEST_ASSERT_EQUAL(MEMORY_POOL_BYTES-i-1, MEMPOOL_GetRemaining());
		TEST_ASSERT_EQUAL(i+1, MEMPOOL_GetUsed());
	}
	TEST_ASSERT_NULL(MEMPOOL_GetBytes(1));
	TEST_ASSERT_EQUAL(0, MEMPOOL_GetRemaining());
	TEST_ASSERT_EQUAL(MEMORY_POOL_BYTES, MEMPOOL_GetUsed());
}

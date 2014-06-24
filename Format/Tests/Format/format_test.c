#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

#include "unity.h"

#include "integertypeenum.h"
#include "format.h"

/*
 * Defines and Typedefs
 */


/*
 * Private Test Variables
 */

void setUp(void)
{
	
}

void tearDown(void)
{

}

void test_U8Format(void)
{
	char buffer[100];
	void * args[5];
	
	uint8_t vals[] = {0, 1, 10, 100, 255};
	
	for(int i = 0; i < 5; ++i)
	{
		args[i] = (void*)&vals[i];
	}
	
	TranslateBuffer("%u, %u, %u, %u, %u", buffer, 0, args, 5);
	TEST_ASSERT_EQUAL_STRING("0, 1, 10, 100, 255", buffer);
}

void test_S8Format(void)
{
	char buffer[100];
	void * args[9];
	int8_t vals[] = {-128, -100, -10, -1, 0, 1, 10, 100, 127};
	
	for(int i = 0; i < 9; ++i)
	{
		args[i] = (void*)&vals[i];
	}
	
	TranslateBuffer("%s, %s, %s, %s, %s, %s, %s, %s, %s", buffer, 0, args, 9);
	TEST_ASSERT_EQUAL_STRING("-128, -100, -10, -1, 0, 1, 10, 100, 127", buffer);
}


void test_U16Format(void)
{
	char buffer[100];
	void * args[7];
	
	uint16_t vals[] = {0, 1, 10, 100, 1000, 10000, 32767};
	
	for(int i = 0; i < 7; ++i)
	{
		args[i] = (void*)&vals[i];
	}
	
	TranslateBuffer("%U, %U, %U, %U, %U, %U, %U", buffer, 0, args, 7);
	TEST_ASSERT_EQUAL_STRING("0, 1, 10, 100, 1000, 10000, 32767", buffer);
}

void test_S16Format(void)
{
	char buffer[100];
	void * args[13];
	
	int16_t vals[] = {-32768, -10000, -1000, -100, -10, -1, 0, 1, 10, 100, 1000, 10000, 32767};
	
	for(int i = 0; i < 13; ++i)
	{
		args[i] = (void*)&vals[i];
	}
	
	TranslateBuffer("%S, %S, %S, %S, %S, %S, %S, %S, %S, %S, %S, %S, %S", buffer, 0, args, 13);
	TEST_ASSERT_EQUAL_STRING("-32768, -10000, -1000, -100, -10, -1, 0, 1, 10, 100, 1000, 10000, 32767", buffer);
}

void test_S32Format(void)
{
	char buffer[100];
	void * args[5];
	
	int32_t vals[] = {(int32_t)INT32_MIN, -1, 0, 1, (int32_t)INT32_MAX};
	
	for(int i = 0; i < 5; ++i)
	{
		args[i] = (void*)&vals[i];
	}
	
	TranslateBuffer("%L, %L, %L, %L, %L", buffer, 0, args, 5);
	TEST_ASSERT_EQUAL_STRING("-2147483648, -1, 0, 1, 2147483647", buffer);
}
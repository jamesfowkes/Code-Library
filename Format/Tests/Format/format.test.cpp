#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/extensions/HelperMacros.h>

#include "integertypeenum.h"
#include "format.h"

#define TOSTR_(x...) #x
#define STRINGIFY(x) TOSTR_(x)

/*
 * Defines and Typedefs
 */


/*
 * Private Test Variables
 */

class FormatTest : public CppUnit::TestFixture  {


	CPPUNIT_TEST_SUITE(FormatTest);
	CPPUNIT_TEST(test_U8Format);
	CPPUNIT_TEST(test_S8Format);
	CPPUNIT_TEST(test_U16Format);
	CPPUNIT_TEST(test_S16Format);
	CPPUNIT_TEST(test_S32Format);
	CPPUNIT_TEST_SUITE_END();

	void test_U8Format(void)
	{
		char buffer[100];
		void * args[5];
		
		#define U8VALS 0, 1, 10, 100, 255
		#define U8VALSSTR STRINGIFY(U8VALS)
		
		uint8_t vals[] = {U8VALS};
		uint8_t count = 0;
		
		for(int i = 0; i < 5; ++i)
		{
			args[i] = (void*)&vals[i];
		}
		
		char const* expectedString = U8VALSSTR;
		count = TranslateBuffer("%u, %u, %u, %u, %u", buffer, 0, args, 5, true);
		CPPUNIT_ASSERT_EQUAL(std::string(expectedString), std::string(buffer));
		CPPUNIT_ASSERT_EQUAL((uint8_t)strlen(expectedString), count);
	}

	void test_S8Format(void)
	{
		char buffer[100];
		void * args[9];
		
		#define S8VALS -128, -100, -10, -1, 0, 1, 10, 100, 127
		#define S8VALSSTR STRINGIFY(S8VALS)
		
		int8_t vals[] = {S8VALS};
		uint8_t count = 0;
		
		for(int i = 0; i < 9; ++i)
		{
			args[i] = (void*)&vals[i];
		}
		
		char const* expectedString = S8VALSSTR;
		count = TranslateBuffer("%s, %s, %s, %s, %s, %s, %s, %s, %s", buffer, 0, args, 9, true);
		CPPUNIT_ASSERT_EQUAL(std::string(expectedString), std::string(buffer));
		CPPUNIT_ASSERT_EQUAL((uint8_t)strlen(expectedString), count);
	}


	void test_U16Format(void)
	{
		char buffer[100];
		void * args[7];
		
		#define U16VALS 0, 1, 10, 100, 1000, 10000, 32767
		#define U16VALSSTR STRINGIFY(U16VALS)
		
		uint16_t vals[] = {U16VALS};
		uint8_t count = 0;
		
		for(int i = 0; i < 7; ++i)
		{
			args[i] = (void*)&vals[i];
		}
		
		char const* expectedString = U16VALSSTR;
		count = TranslateBuffer("%U, %U, %U, %U, %U, %U, %U", buffer, 0, args, 7, true);
		CPPUNIT_ASSERT_EQUAL(std::string(expectedString), std::string(buffer));
		CPPUNIT_ASSERT_EQUAL((uint8_t)strlen(expectedString), count);
	}

	void test_S16Format(void)
	{
		char buffer[100];
		void * args[13];
		
		#define S16VALS -32768, -10000, -1000, -100, -10, -1, 0, 1, 10, 100, 1000, 10000, 32767
		#define S16VALSSTR STRINGIFY(S16VALS)
		
		int16_t vals[] = {S16VALS};
		uint8_t count = 0;
		
		for(int i = 0; i < 13; ++i)
		{
			args[i] = (void*)&vals[i];
		}
		
		char const* expectedString = S16VALSSTR;
		count = TranslateBuffer("%S, %S, %S, %S, %S, %S, %S, %S, %S, %S, %S, %S, %S", buffer, 0, args, 13, true);
		CPPUNIT_ASSERT_EQUAL(std::string(expectedString), std::string(buffer));
		CPPUNIT_ASSERT_EQUAL((uint8_t)strlen(expectedString), count);
	}

	void test_S32Format(void)
	{
		char buffer[100];
		void * args[5];
		
		#define S32VALS -2147483648, -1, 0, 1, 2147483647
		#define S32VALSSTR STRINGIFY(S32VALS)
		
		int32_t vals[] = {S32VALS};
		uint8_t count = 0;
		
		for(int i = 0; i < 5; ++i)
		{
			args[i] = (void*)&vals[i];
		}
		
		char const* expectedString = S32VALSSTR;
		count = TranslateBuffer("%L, %L, %L, %L, %L", buffer, 0, args, 5, true);
		CPPUNIT_ASSERT_EQUAL(std::string(expectedString), std::string(buffer));
		CPPUNIT_ASSERT_EQUAL((uint8_t)strlen(expectedString), count);
	}

	void test_RightAlignDoesNotModifyFullString(void)
	{
		char threeChars[] = "ABC";
		RightAlign(threeChars);
		CPPUNIT_ASSERT_EQUAL("ABC", threeChars);
		
		char twoChars[] = "AB";
		RightAlign(twoChars);
		CPPUNIT_ASSERT_EQUAL("AB", twoChars);
		
		char oneChar[] = "A";
		RightAlign(oneChar);
		CPPUNIT_ASSERT_EQUAL("A", oneChar);
	}

	void test_RightAlignDoesNotModifyRightAlignedString(void)
	{
		char threeChars[] = " ABC";
		RightAlign(threeChars);
		CPPUNIT_ASSERT_EQUAL(" ABC", threeChars);
		
		char twoChars[] = " AB";
		RightAlign(twoChars);
		CPPUNIT_ASSERT_EQUAL(" AB", twoChars);
		
		char oneChar[] = " A";
		RightAlign(oneChar);
		CPPUNIT_ASSERT_EQUAL(" A", oneChar);
	}

	void test_RightAlignAlignsFromLeft(void)
	{
		char threeChars[] = "ABC ";
		RightAlign(threeChars);
		CPPUNIT_ASSERT_EQUAL(" ABC", threeChars);
		
		char twoChars[] = "AB ";
		RightAlign(twoChars);
		CPPUNIT_ASSERT_EQUAL(" AB", twoChars);
		
		char oneChar[] = "A ";
		RightAlign(oneChar);
		CPPUNIT_ASSERT_EQUAL(" A", oneChar);
	}

	void test_RightAlignAlignsFromMiddle(void)
	{
		char threeChars[] = " ABC ";
		RightAlign(threeChars);
		CPPUNIT_ASSERT_EQUAL("  ABC", threeChars);
		
		char twoChars[] = " AB ";
		RightAlign(twoChars);
		CPPUNIT_ASSERT_EQUAL("  AB", twoChars);
		
		char oneChar[] = " A ";
		RightAlign(oneChar);
		CPPUNIT_ASSERT_EQUAL("  A", oneChar);
	}

	void test_RightAlignAlignsWithEmbeddedSpaces(void)
	{
		char threeChars[] = " A B C ";
		RightAlign(threeChars);
		CPPUNIT_ASSERT_EQUAL("  A B C", threeChars);
		
		char twoChars[] = " A B ";
		RightAlign(twoChars);
		CPPUNIT_ASSERT_EQUAL("  A B", twoChars);
	}
};

int main()
{
   CppUnit::TextUi::TestRunner runner;
   
   CPPUNIT_TEST_SUITE_REGISTRATION( FormatTest );

   CppUnit::TestFactoryRegistry &registry = CppUnit::TestFactoryRegistry::getRegistry();

   runner.addTest( registry.makeTest() );
   runner.run();

   return 0;
}

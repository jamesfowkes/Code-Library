#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

#include <avr/io.h>

#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/extensions/HelperMacros.h>

#include "lib_io.h"
#include "lib_swserial.h"

class SwSerialTest : public CppUnit::TestFixture  {

	CPPUNIT_TEST_SUITE(SwSerialTest);
	CPPUNIT_TEST(test_SWSSimpleTransmit);
	CPPUNIT_TEST(test_SWSVarDumpMacro);
	CPPUNIT_TEST(test_SWSTransmit);
	CPPUNIT_TEST_SUITE_END();

	void test_SWSSimpleTransmit(void)
	{
		SWS_SimpleTransmit("HELLO!");
		std::string expected = std::string("HELLO!");
		std::string actual = std::string(SWS_GetTxBuffer());
		CPPUNIT_ASSERT_EQUAL(expected, actual);
	}

	void test_SWSVarDumpMacro(void)
	{
		uint16_t test_var = 34;
		sws_var_dump(test_var, "%U")

		std::string expected = std::string("test_var=34\r\n");
		std::string actual = std::string(SWS_GetTxBuffer());

		CPPUNIT_ASSERT_EQUAL(expected, actual);
	}

	void test_SWSTransmit(void)
	{
		int8_t test_var1 = -7;
		uint16_t test_var2 = 34;
		void * ptrs[] = {&test_var1, &test_var2};
		SWS_Transmit("%s, %U", 0, ptrs, 2);

		std::string expected = std::string("-7, 34");
		std::string actual = std::string(SWS_GetTxBuffer());

		CPPUNIT_ASSERT_EQUAL(expected, actual);
	}

	public:
	void setUp(void)
	{
		SWS_TxInit(IO_PORTB, 0);
		SWS_SetBaudRate(LIB_SWS_BAUD_4800);
	}

	void tearDown(void)
	{

	}

};


int main()
{
   CppUnit::TextUi::TestRunner runner;
   
   CPPUNIT_TEST_SUITE_REGISTRATION( SwSerialTest );

   CppUnit::TestFactoryRegistry &registry = CppUnit::TestFactoryRegistry::getRegistry();

   runner.addTest( registry.makeTest() );
   runner.run();

   return 0;
}
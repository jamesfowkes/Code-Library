#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

#include <avr/io.h>

#include "unity.h"

#include "lib_io.h"
#include "lib_swserial.h"

/*
 * Defines and Typedefs
 */

void setUp(void)
{
	SWS_TxInit(IO_PORTA, 0);
	SWS_SetBaudRate(LIB_SWS_BAUD_4800);
}

void tearDown(void)
{

}

void test_SWSSimpleTransmit(void)
{
	SWS_SimpleTransmit("HELLO!");
	TEST_ASSERT_EQUAL_STRING("HELLO!", SWS_GetTxBuffer());
}

void test_SWSVarDumpMacro(void)
{
	uint16_t test_var = 34;
	sws_var_dump(test_var, "%U")
	TEST_ASSERT_EQUAL_STRING("test_var=34\r\n", SWS_GetTxBuffer());
}

void test_SWSTransmit(void)
{
	int8_t test_var1 = -7;
	uint16_t test_var2 = 34;
	void * ptrs[] = {&test_var1, &test_var2};
	SWS_Transmit("%s, %U", 0, ptrs, 2);
	TEST_ASSERT_EQUAL_STRING("-7, 34\r\n", SWS_GetTxBuffer());
}

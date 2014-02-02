#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "unity.h"

#include "../../llap.h"

static char s_validIDChars[] = "ABCDEFGHIJKLMNOPQRSTUVQXYZ-#@?\\";
static char s_devTooLongName[] = "TestDevice";
static char s_devName[] = "TestDev";
static char s_devTooLongType[] = "DeviceType";
static char s_devType[] = "DevType";

static char s_goodDecimal[] = "1.00";
static char *s_badDecimals[6] = {"1.0", "10.0", "1", "10", "100", "1000"};

static char s_goodSerNum[] = "SERNUM";
static char s_badSerNum[] = "BADSERNUM";

static char s_msgBuffer[] = "------------";

static char s_SendReqBuffer[] = "xxxxxxxxxxxx";

static LLAP_DEVICE s_device;

static void genericMsgHandler(LLAP_GENERIC_MSG_ENUM eMsgType, char * genericStr, char * msgBody)
{
	(void)eMsgType;
	(void)genericStr;
	(void)msgBody;
}

static void applicationMsgHandler(char * msgBody)
{
	(void)msgBody;
}

static void sendRequest(char * msgBody)
{
	sprintf(s_SendReqBuffer, "%s", msgBody);
}

void setUp(void)
{
	LLAP_Init();

}

void makeGoodDevice(void)
{
	s_device.id[0] = 'A';
	s_device.id[1] = 'A';
	s_device.devName = s_devName;
	s_device.devType = s_devType;
	s_device.fwVer = s_goodDecimal;
	s_device.serNum = s_goodSerNum;
	s_device.genericMsgHandler = genericMsgHandler;
	s_device.applicationMsgHandler = applicationMsgHandler;
	s_device.sendRequest = sendRequest;
	s_device.msgBuffer = s_msgBuffer;
	s_device.valid = false;
}

void tearDown(void)
{

}

void test_InitialisingGoodDevice(void)
{
	makeGoodDevice();
	LLAP_StartDevice(&s_device);
	TEST_ASSERT_EQUAL(true, s_device.valid);
	
	uint8_t c1;
	uint8_t c2;
	for (c1 = 0; c1 < strlen(s_validIDChars); ++c1)
	{
		for (c2 = 0; c2 < strlen(s_validIDChars); ++c2)
		{
			makeGoodDevice();
			s_device.id[0] = s_validIDChars[c1];
			s_device.id[1] = s_validIDChars[c2];
			LLAP_StartDevice(&s_device);
			TEST_ASSERT_EQUAL(true, s_device.valid);
		}
	}
}
	
void test_InitialisingBadDevice(void)
{
	
	// Test "initialized to zero" s_device
	LLAP_StartDevice(&s_device);
	TEST_ASSERT_EQUAL(false, s_device.valid);
	
	makeGoodDevice();
	s_device.id[0] = '0';
	s_device.id[1] = '0';
	LLAP_StartDevice(&s_device);
	TEST_ASSERT_EQUAL(false, s_device.valid);
	
	makeGoodDevice();
	s_device.devName = NULL;
	LLAP_StartDevice(&s_device);
	TEST_ASSERT_EQUAL(false, s_device.valid);
	s_device.devName = s_devTooLongName;
	LLAP_StartDevice(&s_device);
	TEST_ASSERT_EQUAL(false, s_device.valid);
	
	makeGoodDevice();
	s_device.devType = NULL;
	LLAP_StartDevice(&s_device);
	TEST_ASSERT_EQUAL(false, s_device.valid);
	s_device.devType = s_devTooLongType;
	LLAP_StartDevice(&s_device);
	TEST_ASSERT_EQUAL(false, s_device.valid);
	
	makeGoodDevice();
	s_device.fwVer = NULL;
	LLAP_StartDevice(&s_device);
	TEST_ASSERT_EQUAL(false, s_device.valid);
	int i = 0;
	for (i = 0; i < 6; ++i)
	{
		s_device.fwVer = s_badDecimals[i];
		LLAP_StartDevice(&s_device);
		TEST_ASSERT_EQUAL(false, s_device.valid);
	}
	
	makeGoodDevice();
	s_device.serNum = NULL;
	LLAP_StartDevice(&s_device);
	TEST_ASSERT_EQUAL(false, s_device.valid);
	s_device.serNum = s_badSerNum;
	LLAP_StartDevice(&s_device);
	TEST_ASSERT_EQUAL(false, s_device.valid);
	
	makeGoodDevice();
	s_device.genericMsgHandler = NULL;
	LLAP_StartDevice(&s_device);
	TEST_ASSERT_EQUAL(false, s_device.valid);
	
	makeGoodDevice();
	s_device.applicationMsgHandler = NULL;
	LLAP_StartDevice(&s_device);
	TEST_ASSERT_EQUAL(false, s_device.valid);
	
	makeGoodDevice();
	s_device.sendRequest = NULL;
	LLAP_StartDevice(&s_device);
	TEST_ASSERT_EQUAL(false, s_device.valid);
	
	makeGoodDevice();
	s_device.valid = true;
	LLAP_StartDevice(&s_device);
	TEST_ASSERT_EQUAL(false, s_device.valid);
}

void test_StartDevice(void)
{
	makeGoodDevice();
	LLAP_StartDevice(&s_device);
	TEST_ASSERT_EQUAL(true, s_device.valid);
	TEST_ASSERT_EQUAL_STRING("aAASTARTED--", s_SendReqBuffer);
}

void test_MakeMessage(void)
{
	makeGoodDevice();
	LLAP_StartDevice(&s_device);
	
	TEST_ASSERT_EQUAL(false, LLAP_MakeMessage(&s_device, "XXXXXXXXXX"));
	TEST_ASSERT_EQUAL(true, LLAP_MakeMessage(&s_device, "XXXXXXXXX"));
	TEST_ASSERT_EQUAL_STRING("aAAXXXXXXXXX", s_device.msgBuffer);
	TEST_ASSERT_EQUAL(true, LLAP_MakeMessage(&s_device, "X"));
	TEST_ASSERT_EQUAL_STRING("aAAX--------", s_device.msgBuffer);
}

/*test_SendBATT(LLAP_DEVICE * dev, char * msg);
test_SendCHDEVID(LLAP_DEVICE * dev, char * msg);
test_SendCYCLE(LLAP_DEVICE * dev);
test_SendINTVL(LLAP_DEVICE * dev, char * msg);
test_SendPANID(LLAP_DEVICE * dev, char * msg);
test_SendREBOOT(LLAP_DEVICE * dev);
test_SendRETRIES(LLAP_DEVICE * dev, char * msg);
test_SendSLEEP(LLAP_DEVICE * dev, char * msg);
test_SendAWAKE(LLAP_DEVICE * dev);
test_SendBATTLOW(LLAP_DEVICE * dev);
test_SendERROR(LLAP_DEVICE * dev, char * msg);
test_SendSLEEPING(LLAP_DEVICE * dev);*/

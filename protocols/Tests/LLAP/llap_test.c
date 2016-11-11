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

static void makeGoodDevice(void);

static void genericMsgHandler(LLAP_GENERIC_MSG_ENUM eMsgType, const char * genericStr, const char * msgBody)
{
	(void)eMsgType;
	(void)genericStr;
	(void)msgBody;
}

static void applicationMsgHandler(const char *  msgBody)
{
	(void)msgBody;
}

static void sendRequest(const char *  msgBody)
{
	sprintf(s_SendReqBuffer, "%s", msgBody);
}

void setUp(void)
{
	LLAP_Init();
	makeGoodDevice();
	LLAP_StartDevice(&s_device);
}

static void makeGoodDevice(void)
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
	memset(&s_device, 0, sizeof(LLAP_DEVICE));
	
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
	TEST_ASSERT_EQUAL(true, s_device.valid);
	TEST_ASSERT_EQUAL_STRING("aAASTARTED--", s_SendReqBuffer);
}

void test_MakeMessage(void)
{
	TEST_ASSERT_EQUAL(false, LLAP_MakeMessageFrom(&s_device, "XXXXXXXXXX"));
	TEST_ASSERT_EQUAL(true, LLAP_MakeMessageFrom(&s_device, "XXXXXXXXX"));
	TEST_ASSERT_EQUAL_STRING("aAAXXXXXXXXX", s_device.msgBuffer);
	TEST_ASSERT_EQUAL(true, LLAP_MakeMessageFrom(&s_device, "X"));
	TEST_ASSERT_EQUAL_STRING("aAAX--------", s_device.msgBuffer);
	
	TEST_ASSERT_EQUAL(false, LLAP_MakeMessageTo(&s_device, "XXXXXXXXXX", "TG"));
	TEST_ASSERT_EQUAL(true, LLAP_MakeMessageTo(&s_device, "XXXXXXXXX", "TG"));
	TEST_ASSERT_EQUAL_STRING("aTGXXXXXXXXX", s_device.msgBuffer);
	TEST_ASSERT_EQUAL(true, LLAP_MakeMessageTo(&s_device, "X", "TG"));
	TEST_ASSERT_EQUAL_STRING("aTGX--------", s_device.msgBuffer);
}

void test_SendOutgoingMessage(void)
{
	TEST_ASSERT_EQUAL(false, LLAP_SendOutgoingMessage(&s_device, "XXXXXXXXXX"));
	TEST_ASSERT_EQUAL(true, LLAP_SendOutgoingMessage(&s_device, "XXXXXXXXX"));
	TEST_ASSERT_EQUAL_STRING("aAAXXXXXXXXX", s_SendReqBuffer);
	TEST_ASSERT_EQUAL(true, LLAP_SendOutgoingMessage(&s_device, "X"));
	TEST_ASSERT_EQUAL_STRING("aAAX--------", s_SendReqBuffer);
	
	TEST_ASSERT_EQUAL(false, LLAP_SendOutgoingMessageTo(&s_device, "XXXXXXXXXX", "TG"));
	TEST_ASSERT_EQUAL(true, LLAP_SendOutgoingMessageTo(&s_device, "XXXXXXXXX", "TG"));
	TEST_ASSERT_EQUAL_STRING("aTGXXXXXXXXX", s_SendReqBuffer);
	TEST_ASSERT_EQUAL(true, LLAP_SendOutgoingMessageTo(&s_device, "X", "TG"));
	TEST_ASSERT_EQUAL_STRING("aTGX--------", s_SendReqBuffer);
}

void test_HandleIncomingMessage(void)
{
	TEST_ASSERT_EQUAL(true, LLAP_HandleIncomingMessage(&s_device, "aAADEVNAME--"));
	TEST_ASSERT_EQUAL_STRING("aAATestDev--", s_SendReqBuffer);
	
	TEST_ASSERT_EQUAL(true, LLAP_HandleIncomingMessage(&s_device, "aAAHELLO----"));
	TEST_ASSERT_EQUAL_STRING("aAAHELLO----", s_SendReqBuffer);
	
	TEST_ASSERT_EQUAL(true, LLAP_HandleIncomingMessage(&s_device, "aAAAPVER----"));
	TEST_ASSERT_EQUAL_STRING("aAAAPVER1.01", s_SendReqBuffer);
	
	TEST_ASSERT_EQUAL(true, LLAP_HandleIncomingMessage(&s_device, "aAADEVTYPE--"));
	TEST_ASSERT_EQUAL_STRING("aAADevType--", s_SendReqBuffer);
	
	TEST_ASSERT_EQUAL(true, LLAP_HandleIncomingMessage(&s_device, "aAAFVER-----"));
	TEST_ASSERT_EQUAL_STRING("aAAFVER1.00-", s_SendReqBuffer);
	
	TEST_ASSERT_EQUAL(true, LLAP_HandleIncomingMessage(&s_device, "aAASER------"));
	TEST_ASSERT_EQUAL_STRING("aAASERSERNUM", s_SendReqBuffer);
}

void test_SendBATT(void)
{
	TEST_ASSERT_EQUAL(true, LLAP_SendBATT(&s_device, "3.30"));
	TEST_ASSERT_EQUAL_STRING("aAABATT3.30-", s_SendReqBuffer);
	TEST_ASSERT_EQUAL(false, LLAP_SendBATT(&s_device, "3"));
	TEST_ASSERT_EQUAL(false, LLAP_SendBATT(&s_device, "33"));
	TEST_ASSERT_EQUAL(false, LLAP_SendBATT(&s_device, "330"));
	TEST_ASSERT_EQUAL(false, LLAP_SendBATT(&s_device, "3.3"));
	TEST_ASSERT_EQUAL(false, LLAP_SendBATT(&s_device, "3.3"));
	TEST_ASSERT_EQUAL(false, LLAP_SendBATT(&s_device, "33.3"));
}

void test_SendCHDEVID(void)
{
	TEST_ASSERT_EQUAL(true, LLAP_SendCHDEVID(&s_device, "TG", "AB"));
	TEST_ASSERT_EQUAL_STRING("aTGCHDEVIDAB", s_SendReqBuffer);
	TEST_ASSERT_EQUAL(false, LLAP_SendCHDEVID(&s_device, "TG", "A"));
	TEST_ASSERT_EQUAL(false, LLAP_SendCHDEVID(&s_device, "TG", "A1"));
	TEST_ASSERT_EQUAL(false, LLAP_SendCHDEVID(&s_device, "TG", "11"));
	TEST_ASSERT_EQUAL(false, LLAP_SendCHDEVID(&s_device, "A", "TG"));
	TEST_ASSERT_EQUAL(false, LLAP_SendCHDEVID(&s_device, "A1", "TG"));
	TEST_ASSERT_EQUAL(false, LLAP_SendCHDEVID(&s_device, "11", "TG"));
	TEST_ASSERT_EQUAL(false, LLAP_SendCHDEVID(&s_device, "A", "A"));
}

void test_SendCYCLE(void)
{
	TEST_ASSERT_EQUAL(true, LLAP_SendCYCLE(&s_device, "TG"));
	TEST_ASSERT_EQUAL_STRING("aTGCYCLE----", s_SendReqBuffer);
}

void test_SendINTVL(void)
{
	int i;
	char intervalString[] = "    ";
	char expectedString[] = "            ";
	for (i = 0; i < 1000; ++i)
	{
		sprintf(intervalString, "%03dS", i);
		sprintf(expectedString, "aTGINTVL%03dS", i);
		TEST_ASSERT_EQUAL(true, LLAP_SendINTVL(&s_device, intervalString, "TG"));
		TEST_ASSERT_EQUAL_STRING(expectedString, s_SendReqBuffer);
	}

	TEST_ASSERT_EQUAL(false, LLAP_SendINTVL(&s_device, "", "TG"));
	TEST_ASSERT_EQUAL(false, LLAP_SendINTVL(&s_device, "9", "TG"));
	TEST_ASSERT_EQUAL(false, LLAP_SendINTVL(&s_device, "99", "TG"));
	TEST_ASSERT_EQUAL(false, LLAP_SendINTVL(&s_device, "999", "TG"));
	TEST_ASSERT_EQUAL(false, LLAP_SendINTVL(&s_device, "999A", "TG"));
}

void test_SendPANID(void)
{
	int i;
	char panidString[] = "    ";
	char expectedString[] = "            ";
	for (i = 0; i <= 0xFFFF; ++i)
	{
		sprintf(panidString, "%04x", i);
		sprintf(expectedString, "aTGPANID%04x", i);
		TEST_ASSERT_EQUAL(true, LLAP_SendPANID(&s_device, panidString, "TG"));
		TEST_ASSERT_EQUAL_STRING(expectedString, s_SendReqBuffer);
	}

	TEST_ASSERT_EQUAL(false, LLAP_SendPANID(&s_device, "", "TG"));
	TEST_ASSERT_EQUAL(false, LLAP_SendPANID(&s_device, "9", "TG"));
	TEST_ASSERT_EQUAL(false, LLAP_SendPANID(&s_device, "99", "TG"));
	TEST_ASSERT_EQUAL(false, LLAP_SendPANID(&s_device, "999", "TG"));
}

void test_SendREBOOT(void)
{
	TEST_ASSERT_EQUAL(true, LLAP_SendREBOOT(&s_device, "TG"));
	TEST_ASSERT_EQUAL_STRING("aTGREBOOT---", s_SendReqBuffer);
}

void test_SendRETRIES(void)
{
	int i;
	char retriesString[] = "  ";
	char expectedString[] = "            ";
	for (i = 0; i < 100; ++i)
	{
		sprintf(retriesString, "%02d", i);
		sprintf(expectedString, "aTGRETRIES%02d", i);
		TEST_ASSERT_EQUAL(true, LLAP_SendRETRIES(&s_device, retriesString, "TG"));
		TEST_ASSERT_EQUAL_STRING(expectedString, s_SendReqBuffer);
	}

	TEST_ASSERT_EQUAL(false, LLAP_SendRETRIES(&s_device, "", "TG"));
	TEST_ASSERT_EQUAL(false, LLAP_SendRETRIES(&s_device, "9", "TG"));
	TEST_ASSERT_EQUAL(false, LLAP_SendRETRIES(&s_device, "AA", "TG"));
}

void test_SendSLEEP(void)
{
	int i;
	char intervalString[] = "    ";
	char expectedString[] = "            ";
	for (i = 0; i < 1000; ++i)
	{
		sprintf(intervalString, "%03dS", i);
		sprintf(expectedString, "aTGSLEEP%03dS", i);
		TEST_ASSERT_EQUAL(true, LLAP_SendSLEEP(&s_device, intervalString, "TG"));
		TEST_ASSERT_EQUAL_STRING(expectedString, s_SendReqBuffer);
	}

	TEST_ASSERT_EQUAL(false, LLAP_SendSLEEP(&s_device, "", "TG"));
	TEST_ASSERT_EQUAL(false, LLAP_SendSLEEP(&s_device, "9", "TG"));
	TEST_ASSERT_EQUAL(false, LLAP_SendSLEEP(&s_device, "99", "TG"));
	TEST_ASSERT_EQUAL(false, LLAP_SendSLEEP(&s_device, "999", "TG"));
	TEST_ASSERT_EQUAL(false, LLAP_SendSLEEP(&s_device, "999A", "TG"));
}

void test_SendAWAKE(void)
{
	TEST_ASSERT_EQUAL(true, LLAP_SendAWAKE(&s_device));
	TEST_ASSERT_EQUAL_STRING("aAAAWAKE----", s_SendReqBuffer);
}

void test_SendBATTLOW(void)
{
	TEST_ASSERT_EQUAL(true, LLAP_SendBATTLOW(&s_device));
	TEST_ASSERT_EQUAL_STRING("aAABATTLOW--", s_SendReqBuffer);
}

void test_SendERROR(void)
{
	TEST_ASSERT_EQUAL(true, LLAP_SendERROR(&s_device, "ERID"));
	TEST_ASSERT_EQUAL_STRING("aAAERRORERID", s_SendReqBuffer);
}

void test_SendSLEEPING(void)
{
	TEST_ASSERT_EQUAL(true, LLAP_SendSLEEPING(&s_device));
	TEST_ASSERT_EQUAL_STRING("aAASLEEPING-", s_SendReqBuffer);
}

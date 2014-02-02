/*
 * Standard Library Includes
 */
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

/*
 * Protocol Library Includes
 */

#include "llap.h"

/*
 * Private Defines and Typedefs
 */

#define MAX_BODY_LENGTH			(9)
#define DECIMAL_STR_LENGTH		(4)
#define INTERVAL_STR_LENGTH		(4)
#define PANID_STR_LENGTH		(4)
#define ID_STR_LENGTH			(2)
#define SERNUM_STR_LENGTH		(6)

#define RETRIES_STR_LENGTH		(2)
#define MAX_ERROR_STR_LENGTH	(4)

#define MAX_DEVICE_NAME_LENGTH	(9)
#define MAX_DEVICE_TYPE_LENGTH	(9)

#define EMPTY_MESSAGE "-----------"

#ifndef LLAP_VERSION
#define LLAP_VERSION "1.01"
#endif

#define GET_BODY_PTR(msg) (&msg[3])

/* These generics messages are handled internally
by the LLAP library and them a message request is
made to the application */
enum pvt_generic_msg_enum
{
	APVER,	
	DEVNAME,
	DEVTYPE,
	FVER,
	HELLO,
	SER,
	MAX_PVT_MSG_ENUM
};
typedef enum pvt_generic_msg_enum PVT_GENERIC_MSG_ENUM;

struct generic_message_string_struct
{
	char * msg;
	uint8_t len;
};
typedef struct generic_message_string_struct GENERIC_MESSAGE_STRING;

/*
 * Private Variables
 */

/* Generic strings for comparison with
incoming messages. Must be kept in order 
of LLAP_GENERIC_MSG_ENUM enumeration, since that
are indexed by that enum */

static GENERIC_MESSAGE_STRING s_generics[] = 
{
	{"BATT", 0},
	{"CHDEVID", 0},
	{"CYCLE", 0},
	{"INTVL", 0},
	{"PANID", 0},
	{"REBOOT", 0},
	{"RETRIES", 0},
	{"SLEEP", 0},
	{"AWAKE", 0},
	{"BATTLOW", 0},
	{"ERROR", 0},
	{"SLEEPING", 0},
	{"STARTED", 0}
};

static GENERIC_MESSAGE_STRING s_pvtGenerics[] = 
{
	{"APVER", 0},
	{"DEVNAME", 0},
	{"DEVTYPE", 0},
	{"FVER", 0},
	{"HELLO", 0},
	{"SER", 0},
};

static char * s_llapVersion = LLAP_VERSION;

/*
 * Private Function Prototypes
 */
static void initBuffer(LLAP_DEVICE * dev);
static void sendMessage(LLAP_DEVICE * dev, char * msgType, uint8_t typeLength);
static void sendMessageWithData(LLAP_DEVICE * dev, char * msgType, uint8_t typeLength, char *data, uint8_t dataLength);

static void padMessageToLength(char *msg);
static bool validateDevice(LLAP_DEVICE * dev);
static bool isValidDecimalString(char * str);
static bool isValidIntervalMessage(char * msg);
static bool isValidPANIDMessage(char * msg);
static bool isValidRETRIESMessage(char * msg);
static bool isValidID(char * id);
static bool isValidMessage(LLAP_DEVICE * dev, char * msg);
static bool tryGenericMessageHandlers(LLAP_DEVICE * dev, char * body);
static bool tryInternalMessageHandlers(LLAP_DEVICE * dev, char * body);

static void handleInternalMessage(PVT_GENERIC_MSG_ENUM ePvtGeneric, LLAP_DEVICE * dev);

/*
 * Public Function Definitions
 */

void LLAP_Init(void)
{
	/* Initialise the length of the generic message strings */
	PVT_GENERIC_MSG_ENUM ePvtGeneric;
	LLAP_GENERIC_MSG_ENUM eGeneric;
	
	for (ePvtGeneric = DEVNAME; ePvtGeneric < MAX_PVT_MSG_ENUM; ++ePvtGeneric)
	{
		s_pvtGenerics[ePvtGeneric].len = strlen( s_pvtGenerics[ePvtGeneric].msg );
	}
	
	for (eGeneric = BATT; eGeneric < MAX_MSG_ENUM; ++eGeneric)
	{
		s_generics[eGeneric].len = strlen( s_generics[eGeneric].msg );
	}
}

bool LLAP_StartDevice(LLAP_DEVICE * dev)
{
	/* First thing to do is check the device information is valid */
	if (dev->valid)
	{
		dev->valid = false; // The device should never be valid before starting!
		return false;
	}
	
	(void)validateDevice(dev);

	/* Then if the device is valid, reguest a started message be sent */
	if (dev->valid)
	{
		initBuffer(dev);
		sendMessage(dev, s_generics[STARTED].msg, s_generics[STARTED].len);
	}
	
	return dev->valid;
}

bool LLAP_HandleIncomingMessage(LLAP_DEVICE * dev, char * msg)
{
	bool bSuccess = false;

	if (dev->valid)
	{
		if (isValidMessage(dev, msg))
		{
			char * body = GET_BODY_PTR(msg);
			
			// Test for LLAP generic messages (internal handling)
			bSuccess = tryInternalMessageHandlers(dev, body);
			
			if (!bSuccess)
			{
				// Test for LLAP generic messages (application handling)
				bSuccess = tryGenericMessageHandlers(dev, body);
			}
					
			if (!bSuccess)
			{
				// This is an application specific message
				dev->applicationMsgHandler(body);
			}
		}
	}
	
	return bSuccess;
}

void LLAP_SendOutgoingMessage(LLAP_DEVICE * dev, char * body)
{
	LLAP_MakeMessage(dev, body);
	dev->sendRequest(dev->msgBuffer);
}
	
bool LLAP_MakeMessage(LLAP_DEVICE * dev, char * body)
{
	bool success = false;
	
	if (dev->valid)
	{
		uint8_t len = strlen(body);
		if (len <= MAX_BODY_LENGTH)
		{
			success = true;
			initBuffer(dev);
			strncpy(GET_BODY_PTR(dev->msgBuffer), body, len+1); // Also copy NULL byte
			padMessageToLength(dev->msgBuffer);
		}
	}
	
	return success;
	
}

void LLAP_SendBATT(LLAP_DEVICE * dev, char * msg)
{
	if (dev->valid && isValidDecimalString(msg))
	{
		sendMessageWithData(dev, s_generics[BATT].msg, s_generics[BATT].len, msg, DECIMAL_STR_LENGTH);
	}
}

void LLAP_SendCHDEVID(LLAP_DEVICE * dev, char * msg)
{
	if (dev->valid && isValidID(msg))
	{
		sendMessageWithData(dev, s_generics[CHDEVID].msg, s_generics[CHDEVID].len, msg, ID_STR_LENGTH);
	}
}

void LLAP_SendCYCLE(LLAP_DEVICE * dev)
{
	if (dev->valid)
	{
		sendMessage(dev, s_generics[CYCLE].msg, s_generics[CYCLE].len);
	}
}

void LLAP_SendINTVL(LLAP_DEVICE * dev, char * msg)
{
	if (dev->valid && isValidIntervalMessage(msg))
	{
		sendMessageWithData(dev, s_generics[CYCLE].msg, s_generics[CYCLE].len, msg, INTERVAL_STR_LENGTH);
	}
}

void LLAP_SendPANID(LLAP_DEVICE * dev, char * msg)
{
	if (dev->valid && isValidPANIDMessage(msg))
	{
		sendMessageWithData(dev, s_generics[PANID].msg, s_generics[PANID].len, msg, PANID_STR_LENGTH);
	}
}

void LLAP_SendREBOOT(LLAP_DEVICE * dev)
{
	if (dev->valid)
	{
		sendMessage(dev, s_generics[REBOOT].msg, s_generics[REBOOT].len);
	}
}

void LLAP_SendRETRIES(LLAP_DEVICE * dev, char * msg)
{
	if (dev->valid && isValidRETRIESMessage(msg))
	{
		sendMessageWithData(dev, s_generics[REBOOT].msg, s_generics[REBOOT].len, msg, RETRIES_STR_LENGTH);
	}
}

void LLAP_SendSLEEP(LLAP_DEVICE * dev, char * msg)
{
	if (dev->valid && isValidIntervalMessage(msg))
	{
		sendMessageWithData(dev, s_generics[SLEEP].msg, s_generics[SLEEP].len, msg, INTERVAL_STR_LENGTH);
	}
}

void LLAP_SendAWAKE(LLAP_DEVICE * dev)
{
	if (dev->valid)
	{
		sendMessage(dev, s_generics[AWAKE].msg, s_generics[AWAKE].len);
	}
}

void LLAP_SendBATTLOW(LLAP_DEVICE * dev)
{
	if (dev->valid)
	{
		sendMessage(dev, s_generics[BATTLOW].msg, s_generics[BATTLOW].len);
	}
}

void LLAP_SendERROR(LLAP_DEVICE * dev, char * msg)
{
	if (dev->valid)
	{
		sendMessageWithData(dev, s_generics[ERROR].msg, s_generics[ERROR].len, msg, MAX_ERROR_STR_LENGTH);
	}
}

void LLAP_SendSLEEPING(LLAP_DEVICE * dev)
{
	if (dev->valid)
	{
		sendMessage(dev, s_generics[SLEEPING].msg, s_generics[SLEEPING].len);
	}
}

/*
 * Private Function Definitions
 */

static bool tryInternalMessageHandlers(LLAP_DEVICE * dev, char * body)
{
	bool msgHandled = false;
	PVT_GENERIC_MSG_ENUM ePvtGeneric;
	
	for (ePvtGeneric = DEVNAME; ((ePvtGeneric < MAX_PVT_MSG_ENUM) && !msgHandled); ++ePvtGeneric)
	{
		if (strncmp(body, s_pvtGenerics[ePvtGeneric].msg, MAX_BODY_LENGTH) == 0)
		{
			handleInternalMessage(ePvtGeneric, dev);
			msgHandled = true;
		}
	}
	
	return msgHandled;
}

static bool tryGenericMessageHandlers(LLAP_DEVICE * dev, char * body)
{
	bool msgHandled = false;
	LLAP_GENERIC_MSG_ENUM eGeneric;
	
	for (eGeneric = BATT; ((eGeneric < MAX_MSG_ENUM) && !msgHandled); ++eGeneric)
	{
		if (strncmp(body, s_generics[eGeneric].msg, MAX_BODY_LENGTH) == 0)
		{
			/* Strings match and device supports this message */
			dev->genericMsgHandler(eGeneric, s_generics[eGeneric].msg, body);
			msgHandled = true;
		}
	}
	
	return msgHandled;
}

static void handleInternalMessage(PVT_GENERIC_MSG_ENUM ePvtGeneric, LLAP_DEVICE * dev)
{
	char * body = GET_BODY_PTR(dev->msgBuffer);
	
	switch(ePvtGeneric)
	{
	case APVER:
		strncpy(body, s_pvtGenerics[APVER].msg, s_pvtGenerics[APVER].len);
		strncpy(body + s_pvtGenerics[APVER].len, s_llapVersion, DECIMAL_STR_LENGTH);
	case DEVNAME:
		strncpy(body, dev->devName, MAX_BODY_LENGTH);
		break;
	case DEVTYPE:
		strncpy(body, dev->devType, MAX_BODY_LENGTH);
		break;
	case FVER:
		strncpy(body, s_pvtGenerics[FVER].msg, s_pvtGenerics[FVER].len);
		strncpy(body + s_pvtGenerics[FVER].len, dev->fwVer, DECIMAL_STR_LENGTH);
		break;
	case HELLO:
		strncpy(body, s_pvtGenerics[HELLO].msg, s_pvtGenerics[HELLO].len);
		break;
	case SER:
		strncpy(body, s_pvtGenerics[SER].msg, s_pvtGenerics[SER].len);
		strncpy(body + s_pvtGenerics[SER].len, dev->serNum, SERNUM_STR_LENGTH);
		break;
	case MAX_PVT_MSG_ENUM:
	default:
		break;
	}
	padMessageToLength(dev->msgBuffer);
	dev->sendRequest(dev->msgBuffer);
}

static void initBuffer(LLAP_DEVICE * dev)
{
	dev->msgBuffer[0] = 'a';
	dev->msgBuffer[1] = dev->id[0];
	dev->msgBuffer[2] = dev->id[1];
	dev->msgBuffer[3] = '\0';
}

static void sendMessage(LLAP_DEVICE * dev, char * msgType, uint8_t typeLength)
{
	sendMessageWithData(dev, msgType, typeLength, NULL, 0);
}

static void sendMessageWithData(LLAP_DEVICE * dev, char * msgType, uint8_t typeLength, char *data, uint8_t dataLength)
{
	if (dev->valid)
	{
		strncpy(GET_BODY_PTR(dev->msgBuffer), msgType, typeLength);
		if (data)
		{
			strncpy(GET_BODY_PTR(dev->msgBuffer) + typeLength, data, dataLength);
		}
		padMessageToLength(dev->msgBuffer);
		dev->sendRequest(dev->msgBuffer);
	}
}

static void padMessageToLength(char *msg)
{
	uint8_t length = strlen(msg);
	
	while (length < LLAP_MESSAGE_LENGTH)
	{
		msg[length] = '-';
		length++;
	}
	
	msg[length] = '\0';
	
}

static bool validateDevice(LLAP_DEVICE * dev)
{
	dev->valid = true; // Assume device is valid
	
	dev->valid &= (dev->devName != NULL);
	dev->valid &= (dev->devType != NULL);
	dev->valid &= (dev->fwVer != NULL);
	dev->valid &= (dev->serNum != NULL);
	dev->valid &= (dev->genericMsgHandler != NULL);
	dev->valid &= (dev->applicationMsgHandler != NULL);
	dev->valid &= (dev->sendRequest != NULL);
	
	if (dev->valid)
	{
		dev->valid &= (strlen(dev->devName) <= MAX_DEVICE_NAME_LENGTH);
		dev->valid &= (strlen(dev->devType) <= MAX_DEVICE_TYPE_LENGTH);
		dev->valid &= (strlen(dev->serNum) == SERNUM_STR_LENGTH);
		dev->valid &= isValidID(dev->id);
		dev->valid &= isValidDecimalString(dev->fwVer);
	}
	
	return dev->valid;
}

static bool isValidDecimalString(char * str)
{
	bool valid = true;
	
	/* Valid decimal numbers are 4 chars long in 9.99 format */

	valid &= (strlen(str) == DECIMAL_STR_LENGTH);
	if (valid)
	{
		valid &= isdigit(str[0]);
		valid &= (str[1] == '.');
		valid &= isdigit(str[2]);
		valid &= isdigit(str[3]);
	}
	
	return valid;
}

static bool isValidIntervalMessage(char * msg)
{
	bool valid = true;
	
	/* Valid interval strings are 4 chars long in 999P format,
	where P (the interval period) can be S, M, H or D */

	valid &= (strlen(msg) == INTERVAL_STR_LENGTH);
	if (valid)
	{
		valid &= isdigit(msg[0]);
		valid &= isdigit(msg[1]);
		valid &= isdigit(msg[2]);
		valid &= ((msg[3] == 'S') || (msg[3] == 'M') || (msg[3] == 'H') || (msg[3] == 'D'));
	}
	
	return valid;
}

static bool isValidPANIDMessage(char * msg)
{
	bool valid = true;
	
	/* Valid PAN ID strings are 4 chars long in XXXX format,
	where X is a hex digit */

	valid &= (strlen(msg) == PANID_STR_LENGTH);
	if (valid)
	{
		valid &= isxdigit(msg[0]);
		valid &= isxdigit(msg[1]);
		valid &= isxdigit(msg[2]);
		valid &= isxdigit(msg[2]);
	}
	
	return valid;
}

static bool isValidRETRIESMessage(char * msg)
{
	bool valid = true;
	
	/* Valid retry strings are 2 chars long in 99 format */

	valid &= (strlen(msg) == RETRIES_STR_LENGTH);
	if (valid)
	{
		valid &= isdigit(msg[0]);
		valid &= isdigit(msg[1]);
	}
	
	return valid;
}

static bool isValidID(char * id)
{
	bool valid[2] = {false, false};
	
	uint8_t c;
	
	for (c = 0; c < 2; ++c)
	{
		valid[c] = isupper(id[c]);
		valid[c] |= (id[c] == '-');
		valid[c] |= (id[c] == '#');
		valid[c] |= (id[c] == '@');
		valid[c] |= (id[c] == '?');
		valid[c] |= (id[c] == '\\');
		valid[c] |= (id[c] == '*');
	}
	
	return valid[0] && valid[1];
}

static bool isValidMessage(LLAP_DEVICE * dev, char * msg)
{
	bool valid = true;
	valid &= (strlen(msg) == 12);
	valid &= (msg[0] == 'a');
	valid &= (msg[1] == dev->id[0]);
	valid &= (msg[2] == dev->id[1]);
	return valid;
}

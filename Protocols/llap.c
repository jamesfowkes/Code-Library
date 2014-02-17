/*
 * Standard Library Includes
 */
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

/*
 * Protocol Library Includes
 */

#include "llap.h"

/*
 * Utility Library Includes
 */

#include "util_memory_placement.h"

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

#define MAKE_MSG_STRUCT(msg) {msg, sizeof(msg)}

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
	const char * msg;
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

#define MAKE_MSG_STRING(id, msg) \
		IN_PMEM(const char id##Str[]) = msg; \
		IN_PMEM(const GENERIC_MESSAGE_STRING id##Msg) = {(const char*)&id##Str, sizeof(msg)}

MAKE_MSG_STRING(battery, "BATT");
MAKE_MSG_STRING(chdevid, "CHDEVID");
MAKE_MSG_STRING(cycle, "CYCLE");
MAKE_MSG_STRING(intvl, "INTVL");
MAKE_MSG_STRING(panid, "PANID");
MAKE_MSG_STRING(reboot, "REBOOT");
MAKE_MSG_STRING(retries, "RETRIES");
MAKE_MSG_STRING(sleep, "SLEEP");
MAKE_MSG_STRING(awake, "AWAKE");
MAKE_MSG_STRING(battlow, "BATTLOW");
MAKE_MSG_STRING(error, "ERROR");
MAKE_MSG_STRING(sleeping, "SLEEPING");
MAKE_MSG_STRING(started, "STARTED");
	
IN_PMEM(GENERIC_MESSAGE_STRING const * const s_generics[]) =
{
	&batteryMsg,
	&chdevidMsg,
	&cycleMsg,
	&intvlMsg,
	&panidMsg,
	&rebootMsg,
	&retriesMsg,
	&sleepMsg,
	&awakeMsg,
	&battlowMsg,
	&errorMsg,
	&sleepingMsg,
	&startedMsg
};

MAKE_MSG_STRING(apver, "APVER");
MAKE_MSG_STRING(devname, "DEVNAME");
MAKE_MSG_STRING(devtype, "DEVTYPE");
MAKE_MSG_STRING(fver, "FVER");
MAKE_MSG_STRING(hello, "HELLO");
MAKE_MSG_STRING(ser, "SER");

IN_PMEM(GENERIC_MESSAGE_STRING const * const s_pvtGenerics[]) =
{
	&apverMsg,
	&devnameMsg,
	&devtypeMsg,
	&fverMsg,
	&helloMsg,
	&serMsg
};

static char * s_llapVersion = LLAP_VERSION;

/*
 * Private Function Prototypes
 */
static void initBuffer(LLAP_DEVICE * dev, char * destinationID);
static bool makeMessage(LLAP_DEVICE * dev, char * body, char * destinationID);
static void sendMessage(LLAP_DEVICE * dev, char * dest, const char * msgType, uint8_t typeLength);
static void sendGenericMessage(LLAP_DEVICE * dev, char * dest, LLAP_GENERIC_MSG_ENUM eMsg);
static void sendMessageWithData(LLAP_DEVICE * dev, char * dest, const char * msgType, uint8_t typeLength, char *data, uint8_t dataLength);
static void sendGenericMessageWithData(LLAP_DEVICE * dev, char * dest, LLAP_GENERIC_MSG_ENUM eMsg, char *data, uint8_t dataLength);

static void padMessageToLength(char *msg);
static bool validateDevice(LLAP_DEVICE * dev);
static bool isValidDecimalString(const char * str);
static bool isValidIntervalMessage(char * msg);
static bool isValidPANIDMessage(char * msg);
static bool isValidRETRIESMessage(char * msg);
static bool isValidID(char * id);
static bool isValidIDChar(char c);
static bool isValidMessage(LLAP_DEVICE * dev, char * msg);
static bool tryGenericMessageHandlers(LLAP_DEVICE * dev, char * body);
static bool tryInternalMessageHandlers(LLAP_DEVICE * dev, char * body);

static void handleInternalMessage(PVT_GENERIC_MSG_ENUM ePvtGeneric, LLAP_DEVICE * dev);

/*
 * Public Function Definitions
 */

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
		sendGenericMessage(dev, NULL, STARTED);
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

bool LLAP_SendOutgoingMessage(LLAP_DEVICE * dev, char * body)
{
	return LLAP_SendOutgoingMessageTo(dev, body, NULL);
}

bool LLAP_SendOutgoingMessageTo(LLAP_DEVICE * dev, char * body, char * destinationID)
{
	bool success = makeMessage(dev, body, destinationID);
	
	if (success)
	{
		dev->sendRequest(dev->msgBuffer);
	}
	
	return success;
}
	
bool LLAP_MakeMessageTo(LLAP_DEVICE * dev, char * body, char * destinationID)
{
	return makeMessage(dev, body, destinationID);
}

bool LLAP_MakeMessageFrom(LLAP_DEVICE * dev, char * body)
{
	return makeMessage(dev, body, NULL);
}

bool LLAP_SendBATT(LLAP_DEVICE * dev, char * msg)
{
	bool success = dev->valid && isValidDecimalString(msg);
	if (success)
	{
		sendGenericMessageWithData(dev, NULL, BATT, msg, DECIMAL_STR_LENGTH);
	}
	return success;
}

bool LLAP_SendCHDEVID(LLAP_DEVICE * dev, char *targetID, char * newID)
{
	bool success = dev->valid && isValidID(targetID) && isValidID(newID);
	if (success)
	{
		sendGenericMessageWithData(dev, targetID, CHDEVID, newID, ID_STR_LENGTH);
	}
	return success;
}

bool LLAP_SendCYCLE(LLAP_DEVICE * dev, char *targetID)
{
	bool success = dev->valid && isValidID(targetID);
	if (success)
	{
		sendGenericMessage(dev, targetID, CYCLE);
	}
	return success;
}

bool LLAP_SendINTVL(LLAP_DEVICE * dev, char * msg, char *targetID)
{
	bool success = dev->valid && isValidIntervalMessage(msg) && isValidID(targetID);
	if (success)
	{
		sendGenericMessageWithData(dev, targetID, INTVL, msg, INTERVAL_STR_LENGTH);
	}
	return success;
}

bool LLAP_SendPANID(LLAP_DEVICE * dev, char * msg, char *targetID)
{
	bool success = dev->valid && isValidPANIDMessage(msg) && isValidID(targetID);
	if (success)
	{
		sendGenericMessageWithData(dev, targetID, PANID, msg, PANID_STR_LENGTH);
	}
	return success;
}

bool LLAP_SendREBOOT(LLAP_DEVICE * dev, char *targetID)
{
	bool success = dev->valid;
	if (dev->valid)
	{
		sendGenericMessage(dev, targetID, REBOOT);
	}
	return success;
}

bool LLAP_SendRETRIES(LLAP_DEVICE * dev, char * msg, char *targetID)
{
	bool success = dev->valid && isValidRETRIESMessage(msg) && isValidID(targetID);
	if (success)
	{
		sendGenericMessageWithData(dev, targetID, RETRIES, msg, RETRIES_STR_LENGTH);
	}
	return success;
}

bool LLAP_SendSLEEP(LLAP_DEVICE * dev, char * msg, char *targetID)
{
	bool success = dev->valid && isValidIntervalMessage(msg) && isValidID(targetID);
	if (success)
	{
		sendGenericMessageWithData(dev, targetID, SLEEP, msg, INTERVAL_STR_LENGTH);
	}
	return success;
}

bool LLAP_SendAWAKE(LLAP_DEVICE * dev)
{
	bool success = dev->valid;
	if (dev->valid)
	{
		sendGenericMessage(dev, NULL, AWAKE);
	}
	return success;
}

bool LLAP_SendBATTLOW(LLAP_DEVICE * dev)
{
	bool success = dev->valid;
	if (dev->valid)
	{
		sendGenericMessage(dev, NULL, BATTLOW);
	}
	return success;
}

bool LLAP_SendERROR(LLAP_DEVICE * dev, char * msg)
{
	bool success = dev->valid;
	if (dev->valid)
	{
		sendGenericMessageWithData(dev, NULL, ERROR, msg, MAX_ERROR_STR_LENGTH);
	}
	return success;
}

bool LLAP_SendSLEEPING(LLAP_DEVICE * dev)
{
	bool success = dev->valid;
	if (dev->valid)
	{
		sendGenericMessage(dev, NULL, SLEEPING);
	}
	return success;
}

/*
 * Private Function Definitions
 */

static bool tryInternalMessageHandlers(LLAP_DEVICE * dev, char * body)
{
	bool msgHandled = false;
	PVT_GENERIC_MSG_ENUM ePvtGeneric;
	
	for (ePvtGeneric = APVER; ((ePvtGeneric < MAX_PVT_MSG_ENUM) && !msgHandled); ++ePvtGeneric)
	{
		if (STRNCMP(body, s_pvtGenerics[ePvtGeneric]->msg, s_pvtGenerics[ePvtGeneric]->len) == 0)
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
		if (STRNCMP(body, s_generics[eGeneric]->msg, MAX_BODY_LENGTH) == 0)
		{
			/* Strings match and device supports this message.
			Pass a RAM copy of the message to the application*/
			char msgRamCopy[LLAP_MESSAGE_LENGTH];
			PMEM_STRNCPY(msgRamCopy, s_generics[eGeneric]->msg, s_generics[eGeneric]->len);
			dev->genericMsgHandler(eGeneric, msgRamCopy, body);
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
		PMEM_STRNCPY(body, s_pvtGenerics[APVER]->msg, s_pvtGenerics[APVER]->len);
		strncpy(body + s_pvtGenerics[APVER]->len, s_llapVersion, DECIMAL_STR_LENGTH);
		break;
	case DEVNAME:
		strncpy(body, dev->devName, MAX_BODY_LENGTH);
		break;
	case DEVTYPE:
		strncpy(body, dev->devType, MAX_BODY_LENGTH);
		break;
	case FVER:
		PMEM_STRNCPY(body, s_pvtGenerics[FVER]->msg, s_pvtGenerics[FVER]->len);
		strncpy(body + s_pvtGenerics[FVER]->len, dev->fwVer, DECIMAL_STR_LENGTH);
		break;
	case HELLO:
		PMEM_STRNCPY(body, s_pvtGenerics[HELLO]->msg, MAX_BODY_LENGTH);
		break;
	case SER:
		PMEM_STRNCPY(body, s_pvtGenerics[SER]->msg, s_pvtGenerics[SER]->len);
		strncpy(body + s_pvtGenerics[SER]->len, dev->serNum, SERNUM_STR_LENGTH);
		break;
	case MAX_PVT_MSG_ENUM:
	default:
		break;
	}
	padMessageToLength(dev->msgBuffer);
	dev->sendRequest(dev->msgBuffer);
}

static void initBuffer(LLAP_DEVICE * dev, char * destinationID)
{
	dev->msgBuffer[0] = 'a';
	dev->msgBuffer[1] = destinationID ? destinationID[0] : dev->id[0];
	dev->msgBuffer[2] = destinationID ? destinationID[1] : dev->id[1];
	dev->msgBuffer[3] = '\0';
}

static bool makeMessage(LLAP_DEVICE * dev, char * body, char * destinationID)
{
	bool success = false;
	
	if (dev->valid)
	{
		uint8_t len = strlen(body);
		if (len <= MAX_BODY_LENGTH)
		{
			success = true;
			initBuffer(dev, destinationID);
			strncpy(GET_BODY_PTR(dev->msgBuffer), body, len+1); // Also copy NULL byte
			padMessageToLength(dev->msgBuffer);
		}
	}
	return success;
}

static void sendGenericMessage(LLAP_DEVICE * dev, char * dest, LLAP_GENERIC_MSG_ENUM eMsg)
{
	const char * msgType = s_generics[eMsg]->msg;
	uint8_t typeLength = s_generics[eMsg]->len;

	char msgRamCopy[LLAP_MESSAGE_LENGTH];
	PMEM_STRNCPY(msgRamCopy, msgType, typeLength);
	
	sendMessage(dev, dest, msgRamCopy, typeLength);
}

static void sendMessage(LLAP_DEVICE * dev, char * dest, const char * msgType, uint8_t typeLength)
{
	sendMessageWithData(dev, dest, msgType, typeLength, NULL, 0);
}

static void sendGenericMessageWithData(LLAP_DEVICE * dev, char * dest, LLAP_GENERIC_MSG_ENUM eMsg, char *data, uint8_t dataLength)
{
	const char * msgType = s_generics[eMsg]->msg;
	uint8_t typeLength = s_generics[eMsg]->len;

	char msgRamCopy[LLAP_MESSAGE_LENGTH];
	PMEM_STRNCPY(msgRamCopy, msgType, typeLength);
	
	sendMessageWithData(dev, dest, msgRamCopy, typeLength, data, dataLength);
}

static void sendMessageWithData(LLAP_DEVICE * dev, char * dest, const char * msgType, uint8_t typeLength, char *data, uint8_t dataLength)
{
	if (dev->valid)
	{
		initBuffer(dev, dest);
		strncpy(GET_BODY_PTR(dev->msgBuffer), msgType, typeLength+1); // Also copy NULL byte
		if (data)
		{
			strncpy(GET_BODY_PTR(dev->msgBuffer) + typeLength, data, dataLength+1); // Also copy NULL byte
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

static bool isValidDecimalString(const char * str)
{
	bool valid = true;
	
	/* Valid decimal numbers are 4 chars long in 9.99 format */

	valid &= (strlen(str) == DECIMAL_STR_LENGTH);
	if (valid)
	{
		valid &= (bool)isdigit(str[0]);
		valid &= (str[1] == '.');
		valid &= (bool)isdigit(str[2]);
		valid &= (bool)isdigit(str[3]);
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
		valid &= (bool)isdigit(msg[0]);
		valid &= (bool)isdigit(msg[1]);
		valid &= (bool)isdigit(msg[2]);
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
		valid &= (bool)isxdigit(msg[0]);
		valid &= (bool)isxdigit(msg[1]);
		valid &= (bool)isxdigit(msg[2]);
		valid &= (bool)isxdigit(msg[3]);
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
		valid &= (bool)isdigit(msg[0]);
		valid &= (bool)isdigit(msg[1]);
	}
	
	return valid;
}

static bool isValidID(char * id)
{
	return isValidIDChar(id[0]) && isValidIDChar(id[1]);
}

static bool isValidIDChar(char c)
{
	bool valid = false;
	
	valid |= isupper(c);
	valid |= (c == '-');
	valid |= (c == '#');
	valid |= (c == '@');
	valid |= (c == '?');
	valid |= (c == '\\');
	valid |= (c == '*');
	
	return valid;
}

static bool isValidMessage(LLAP_DEVICE * dev, char * msg)
{
	bool valid = true;
	valid &= (strlen(msg) == LLAP_MESSAGE_LENGTH);
	valid &= (msg[0] == 'a');
	valid &= (msg[1] == dev->id[0]);
	valid &= (msg[2] == dev->id[1]);
	return valid;
}

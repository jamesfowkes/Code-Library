#ifndef _LLAP_H_
#define _LLAP_H_

/* Defines and Typedefs */

#define LLAP_MESSAGE_LENGTH	(12)

enum llap_generic_msg_enum
{
	BATT,
	CHDEVID,
	CYCLE,
	INTVL,
	PANID,
	REBOOT,
	RETRIES,
	SLEEP,
	AWAKE,
	BATTLOW,
	ERROR,
	SLEEPING,
	STARTED,
	MAX_MSG_ENUM
};
typedef enum llap_generic_msg_enum LLAP_GENERIC_MSG_ENUM;

typedef void	(*LLAP_GENERIC_HANDLER)(LLAP_GENERIC_MSG_ENUM eMsgType, char * genericStr, char * msgBody);
typedef void	(*LLAP_APPLICATION_HANDLER)(char * msgBody);
typedef void 	(*LLAP_SEND_FN)(char * msgBody);

struct llap_device
{
	char id[2];
	char msgDestination[2];
	char * devName;
	char * devType;
	char * fwVer;
	char * serNum;
	LLAP_GENERIC_HANDLER genericMsgHandler; // Handler for generic messages that require application to supply replies
	LLAP_APPLICATION_HANDLER applicationMsgHandler; // Handler for messages that the protocol does not recognise
	LLAP_SEND_FN sendRequest;	// Function by which the LLAP library can request a message to be sent
	char * msgBuffer;
	
	/* This boolean is set by the LLAP library and should not be written by the application software
	EXCEPT to initialise it to FALSE at power-on. The safest way is to declare all LLAP device structs as static. */
	bool valid;
};
typedef struct llap_device LLAP_DEVICE;

/*
 * Public Function Prototypes
 */

void LLAP_Init(void);
bool LLAP_StartDevice(LLAP_DEVICE * dev);
bool LLAP_HandleIncomingMessage(LLAP_DEVICE * dev, char * msg);
bool LLAP_SendOutgoingMessage(LLAP_DEVICE * dev, char * body);
bool LLAP_SendOutgoingMessageTo(LLAP_DEVICE * dev, char * body, char * destinationID);
bool LLAP_MakeMessageTo(LLAP_DEVICE * dev, char * body, char * destinationID);
bool LLAP_MakeMessageFrom(LLAP_DEVICE * dev, char * body);

bool LLAP_SendBATT(LLAP_DEVICE * dev, char * msg);
bool LLAP_SendCHDEVID(LLAP_DEVICE * dev, char *targetID, char * newID);
bool LLAP_SendCYCLE(LLAP_DEVICE * dev, char *targetID);
bool LLAP_SendINTVL(LLAP_DEVICE * dev, char * msg, char *targetID);
bool LLAP_SendPANID(LLAP_DEVICE * dev, char * msg, char *targetID);
bool LLAP_SendREBOOT(LLAP_DEVICE * dev, char *targetID);
bool LLAP_SendRETRIES(LLAP_DEVICE * dev, char * msg, char *targetID);
bool LLAP_SendSLEEP(LLAP_DEVICE * dev, char * msg, char *targetID);
bool LLAP_SendAWAKE(LLAP_DEVICE * dev);
bool LLAP_SendBATTLOW(LLAP_DEVICE * dev);
bool LLAP_SendERROR(LLAP_DEVICE * dev, char * msg);
bool LLAP_SendSLEEPING(LLAP_DEVICE * dev);

#endif

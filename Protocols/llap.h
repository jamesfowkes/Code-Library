
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
void LLAP_HandleIncomingMessage(LLAP_DEVICE * dev, char * msg);
void LLAP_SendOutgoingMessage(LLAP_GENERIC_MSG_ENUM eMsg, char * body);

void LLAP_SendBATT(LLAP_DEVICE * dev, char * msg);
void LLAP_SendCHDEVID(LLAP_DEVICE * dev, char * msg);
void LLAP_SendCYCLE(LLAP_DEVICE * dev);
void LLAP_SendINTVL(LLAP_DEVICE * dev, char * msg);
void LLAP_SendPANID(LLAP_DEVICE * dev, char * msg);
void LLAP_SendREBOOT(LLAP_DEVICE * dev);
void LLAP_SendRETRIES(LLAP_DEVICE * dev, char * msg);
void LLAP_SendSLEEP(LLAP_DEVICE * dev, char * msg);
void LLAP_SendAWAKE(LLAP_DEVICE * dev);
void LLAP_SendBATTLOW(LLAP_DEVICE * dev);
void LLAP_SendERROR(LLAP_DEVICE * dev, char * msg);
void LLAP_SendSLEEPING(LLAP_DEVICE * dev);

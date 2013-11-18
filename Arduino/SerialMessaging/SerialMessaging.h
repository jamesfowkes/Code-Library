#ifndef _SERIAL_MESSAGING_H_
#define _SERIAL_MESSAGING_H_

#define MAX_MESSAGES_PER_HANDLE 2
#define MSG_BUFFER_LEN 200

struct message_handler_struct
{
	void(*fn)(String *pMessage);
	char const * const startsWith[MAX_MESSAGES_PER_HANDLE];
	char const * const pHelp;
};
typedef struct message_handler_struct MESSAGE_HANDLER;

typedef void (*SERIALCALLBACK)(String *pMessage);

class SerialMessaging
{
	public:
		SerialMessaging(MESSAGE_HANDLER * pMessageHandler, uint8_t count);
		SerialMessaging(SERIALCALLBACK cb);

		void SerialEvent(void);
		~SerialMessaging();
		void Begin(unsigned long baudrate);
		
		template <class T>
		void Print(T toPrint) { Serial.print(toPrint); }

		template <class T>
		void Println(T toPrint) { Serial.println(toPrint); }

	private:

		void serialMessageHandler(String * pMessage);
		void printHelpMessages(void);

		MESSAGE_HANDLER * m_pMessageHandlers;
		uint8_t m_HandlerCount;
		char m_MessageBuffer[MSG_BUFFER_LEN];
		char *m_pBuffer;

		SERIALCALLBACK m_Callback;
};
#endif

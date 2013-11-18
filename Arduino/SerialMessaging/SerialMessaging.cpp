#include <Arduino.h>
#include <ArduinoUtil.h>

#include "SerialMessaging.h"

SerialMessaging::SerialMessaging(MESSAGE_HANDLER * pMessageHandlers, uint8_t count)
{
	m_pMessageHandlers = pMessageHandlers;
	m_pBuffer = m_MessageBuffer;
	m_HandlerCount = count;
	m_Callback = NULL;
}

SerialMessaging::SerialMessaging(SERIALCALLBACK cb)
{
	m_Callback = cb;

	m_pMessageHandlers = NULL;
	m_pBuffer = m_MessageBuffer;
	m_HandlerCount = 0;
}

SerialMessaging::~SerialMessaging(){}

void SerialMessaging::Begin(unsigned long baudrate)
{
	Serial.begin(baudrate);
}

void SerialMessaging::SerialEvent(void)
{
	while (Serial.available())
	{
		char inChar = (char)Serial.read();

		// if the incoming character is a newline, invoke handler
		if (inChar == '\n' || inChar == '\r')
		{

			String sMessage = String(m_MessageBuffer);
			sMessage.toUpperCase();

			if (m_pMessageHandlers)
			{
				serialMessageHandler(&sMessage);
			}

			if (m_Callback)
			{
				m_Callback(&sMessage);
			}

			m_pBuffer = m_MessageBuffer;
			*m_pBuffer = '\0';
		}
		else
		{
			if (strlen(m_MessageBuffer) < MSG_BUFFER_LEN)
			{
				*m_pBuffer++ = inChar;
				*m_pBuffer = '\0';
			}
		}
	}
}

/*
 *	()serialMessageHandler
 **		When complete message comes in, this is called for handling
 */
void SerialMessaging::serialMessageHandler(String * pMessage)
{
	uint8_t iHandler = 0;
	uint8_t i;

	if (pMessage->startsWith("?"))
	{
		this->printHelpMessages();
	}

	for (iHandler = 0; iHandler < m_HandlerCount; ++iHandler)
	{
		MESSAGE_HANDLER * handler = &m_pMessageHandlers[iHandler];
		bool msgFound = false;

		for (i = 0; i < MAX_MESSAGES_PER_HANDLE; ++i)
		{
			if (handler->startsWith[i][0] != '\0')
			{
				msgFound |= pMessage->startsWith(handler->startsWith[i]);
			}
		}

		if (msgFound && handler->fn)
		{
			handler->fn(pMessage);
		}
	}
}

void SerialMessaging::printHelpMessages(void)
{
	uint8_t iHandler = 0;
	uint8_t i;

	Serial.println("Available commands:");

	for (iHandler = 0; iHandler < m_HandlerCount; ++iHandler)
	{
		MESSAGE_HANDLER * handler = &m_pMessageHandlers[iHandler];

		for (i = 0; i < MAX_MESSAGES_PER_HANDLE; ++i)
		{
			if (handler->startsWith[i][0] != '\0')
			{
				Serial.print("\"");
				Serial.print(handler->startsWith[i]);
				Serial.print("\"");
				if(!islast(i, MAX_MESSAGES_PER_HANDLE))
				{
					Serial.print(", ");
				}
			}
		}

		Serial.print(": ");
		Serial.println(handler->pHelp);
	}

	Serial.println("\"?\": Print this help text");
}

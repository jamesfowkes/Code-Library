#ifndef _SERIAL_STEPPER_H_
#define _SERIAL_STEPPER_H_

#include <Arduino.h>

#define CMD_STOP		{'S', 'T', 'O'}
#define CMD_RUN			{'R', 'U', 'N'}
#define CMD_SET_RATE	{'R', 'T', 'E'}
#define CMD_SET_SPD		{'S', 'P', 'D'}
#define CMD_STEP		{'S', 'T', 'E'}

enum eCommand
{
	eSTOP,
	eRUN,
	eSET_RATE,
	eSET_SPD,
	eSTEP,
	MAX_COMMANDS
};
typedef enum eCommand eCOMMAND;

typedef void(*COMMAND_HANDLER)(int cmdValue);

struct command
{
	char c[3];
};
typedef struct command COMMAND;

struct command_packet
{
	COMMAND cmd;
	char value[4];
};
typedef struct command_packet COMMAND_PACKET;

class StepperCommander
{
	public:
		StepperCommander(COMMAND_HANDLER * pHandlers);
		void HandlePacket(COMMAND_PACKET * packet);
		void HandlePacket(char packetChars[]);
		
		~StepperCommander();
	
	private:
		COMMAND_HANDLER * m_pHandlers;
};

#endif

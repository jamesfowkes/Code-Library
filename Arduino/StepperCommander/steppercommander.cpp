#include <stdint.h>
#include <cstdlib>

#include "steppercommander.h"

static COMMAND s_commandMap[MAX_COMMANDS] = {
	{CMD_STOP},
	{CMD_RUN},
	{CMD_SET_RATE},
	{CMD_SET_SPD},
	{CMD_STEP}
};

static eCOMMAND commandFromPacket(COMMAND * pPacketCmd);
static bool matchPacketCmd(COMMAND * pCmd, COMMAND * pPacketCmd);

static eCOMMAND commandFromPacket(COMMAND * pPacketCmd)
{
	bool matched = false;
	uint8_t eCmd = (uint8_t)eSTOP;
	for (; eCmd < MAX_COMMANDS; ++eCmd)
	{
		matched = matchPacketCmd(&s_commandMap[eCmd], pPacketCmd);
		if (matched) {break;}
	}
	return matched ? (eCOMMAND)eCmd : MAX_COMMANDS;
}

static bool matchPacketCmd(COMMAND * pCmd, COMMAND * pPacketCmd)
{
	bool matched = true;
	matched &= (pCmd->c[0] == pPacketCmd->c[0]);
	matched &= (pCmd->c[1] == pPacketCmd->c[1]);
	matched &= (pCmd->c[2] == pPacketCmd->c[2]);
	
	return matched;
}

StepperCommander::StepperCommander(COMMAND_HANDLER * pHandlers)
{
	m_pHandlers = pHandlers;
}

StepperCommander::~StepperCommander(){}

void StepperCommander::HandlePacket(char packetChars[])
{
	HandlePacket((COMMAND_PACKET *)packetChars);
}

void StepperCommander::HandlePacket(COMMAND_PACKET * packet)
{
	eCOMMAND eCmd = commandFromPacket(&packet->cmd);
	
	if (eCmd != MAX_COMMANDS)
	{
		if (m_pHandlers[eCmd])
		{
			int value = atoi(packet->value);
			m_pHandlers[eCmd](value);
		}
	}
}

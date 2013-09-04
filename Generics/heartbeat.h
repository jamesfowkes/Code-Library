#ifndef _HEARTBEAT_H_
#define _HEARTBEAT_H_

void Heartbeat_Init(uint8_t msTick, uint16_t msBeat);
bool Heartbeat_Tick(void);

#endif

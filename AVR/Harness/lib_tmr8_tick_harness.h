#ifndef _LIB_TMR8_TICK_HARNESS_H_
#define _LIB_TMR8_TICK_HARNESS_H_
void TMR8_Tick_Harness_Kick(
	uint8_t ms, TMR8_TICK_CONFIG * TimerConfigs[], TMR8_DELAY_CONFIG * DelayConfigs[],
	uint8_t s_TimerCount, uint8_t s_DelayCount,
	uint16_t *pMSCounter, uint16_t *pSecondsSinceInit);
#endif

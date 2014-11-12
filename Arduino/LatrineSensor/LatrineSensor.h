#ifndef _LATRINE_SENSOR_H_
#define _LATRINE_SENSOR_H_

#include <stdint.h>

#ifndef UNITY_TEST
#include <Arduino.h>
#endif

typedef void(*LS_START_CB)(void);
typedef void(*LS_END_CB)(uint16_t durationInSeconds);

class LatrineSensor
{
public:
	LatrineSensor(LS_START_CB pFnStart, LS_END_CB pFnEnd, bool bEmitDebugInfo = false);
	#ifndef UNITY_TEST
	uint16_t Update(void);
	#else
	uint16_t Update(uint16_t count);
	#endif
	void Setup(void);
	bool IsCalibrating(void);
	bool IsFlushing(void);
	
	uint16_t GetFlushStartThreshold(void);
	uint16_t GetFlushEndThreshold(void);
private:

	enum application_states
	{
		STATE_CALIBRATING,
		STATE_IDLE,
		STATE_FLUSHING,
	};
	
	LS_START_CB s_pFnStart;
	LS_END_CB s_pFnEnd;
	
	bool s_bEmitDebugInfo;

	uint32_t s_averageHigh;
	uint32_t s_minimumLow;

	uint32_t s_flushStartThreshold;
	uint32_t s_flushEndThreshold;

	long s_calibCount;
	
	uint32_t s_LastUpdate;
	uint32_t s_flushDurationMs;
	
	uint16_t s_flushState;

	#ifndef UNITY_TEST
	uint16_t updateTask(void);
	#else
	uint16_t updateTask(uint16_t count);
	#endif
	
	void updateCalibration(uint16_t lastCount);
	void updateStartThreshold(uint16_t lastCount);
	void updateEndThreshold(uint16_t lastCount);
	void handleIdleFlush(uint16_t count);
	void handleFlushing(uint16_t count);
	
	void resetFlushDuration(void);
	void updateFlushDuration(void);
	uint16_t getFlushDurationInSeconds(void);
	void emitDebugInfo(uint16_t lastCount);
};


#endif

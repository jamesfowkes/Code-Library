/*
 * Includes
 */
 
#include <LatrineSensor.h>

#ifndef UNITY_TEST
#include <util/atomic.h>
#endif

#ifndef min
#define min(a, b) (((a) < (b)) ? (a) : (b))
#endif

/*
 * Constants (might become private class variables?)
 */

static const uint16_t MINIMUM_FREQ_DIFF = 200;
static const unsigned long UPDATE_TICK_MS = 1000;

LatrineSensor::LatrineSensor(LS_START_CB pFnStart, LS_END_CB pFnEnd, bool bEmitDebugInfo)
{
	s_pFnStart = pFnStart;
	s_pFnEnd = pFnEnd;
	s_bEmitDebugInfo = bEmitDebugInfo;
	
	s_averageHigh = 0;
	s_minimumLow = 0;

	s_flushStartThreshold = 0;
	s_flushEndThreshold = 0;

	s_calibCount = 0;
	
	s_LastUpdate = 0;
	s_flushDurationMs = 0;
	
	s_flushState = STATE_CALIBRATING;

}

void LatrineSensor::Setup(void)
{
	#ifndef UNITY_TEST
	/* Timer1 (16-bit timer) setup) 
	- Counts on T1 pin, which is PD5 which is Arduino pin 5 */
	TCCR1A = 0; // Output compare off, waveform generation off
	TCCR1B = 0x06; // Input capture off, waveform generation off, clock source external falling edge
	TCNT1 = 0;
	#endif
}

#ifndef UNITY_TEST
uint16_t LatrineSensor::Update(void){ return updateTask(); }
#else
uint16_t LatrineSensor::Update(uint16_t count) { return updateTask(count); }
#endif

bool LatrineSensor::IsCalibrating(void) { return s_flushState == STATE_CALIBRATING; }
bool LatrineSensor::IsFlushing(void) { return s_flushState == STATE_FLUSHING; }
	
void LatrineSensor::emitDebugInfo(uint16_t lastCount)
{
	#ifndef UNITY_TEST
	Serial.print("A=");
	Serial.print(s_averageHigh);
	Serial.print(", C=");
	Serial.print(lastCount);
	Serial.print(", FS=");
	Serial.print(s_flushStartThreshold);
	Serial.print(", FE=");
	Serial.print(s_flushEndThreshold);
	Serial.print(", S=");
	Serial.println(s_flushState);
	#else
	(void)lastCount;
	#endif
}

#ifndef UNITY_TEST
uint16_t LatrineSensor::updateTask(void)
#else
uint16_t LatrineSensor::updateTask(uint16_t count)
#endif
{
	#ifndef UNITY_TEST
	volatile uint16_t count;
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		count = TCNT1;
		TCNT1 = 0;
	}
	#endif
	
	switch(s_flushState)
	{
	case STATE_CALIBRATING:
		updateCalibration(count);
		break;
	case STATE_IDLE:
		handleIdleFlush(count);
		break;
	case STATE_FLUSHING:
		handleFlushing(count);
		break;
	}
	
	if (s_bEmitDebugInfo) { emitDebugInfo(count); }
	
	return count;
}

uint16_t LatrineSensor::GetFlushStartThreshold(void)
{
	return s_flushStartThreshold;
}

uint16_t LatrineSensor::GetFlushEndThreshold(void)
{
	return s_flushEndThreshold;
}

void LatrineSensor::updateCalibration(uint16_t lastCount)
{
	static uint32_t calibCount = 0;

	s_calibCount += lastCount;

	if (++calibCount == 5)
	{
		s_averageHigh = (s_calibCount / 5);
		updateStartThreshold(s_averageHigh);
		s_flushState = STATE_IDLE;
	}
}

void LatrineSensor::updateStartThreshold(uint16_t lastCount)
{
	s_averageHigh = (s_averageHigh + lastCount) / 2;
	s_flushStartThreshold = s_averageHigh - MINIMUM_FREQ_DIFF;
}

void LatrineSensor::updateEndThreshold(uint16_t lastCount)
{
	s_minimumLow = min(lastCount, s_minimumLow);
	s_flushEndThreshold = s_minimumLow + MINIMUM_FREQ_DIFF;
}

void LatrineSensor::handleIdleFlush(uint16_t count)
{
	if (count < s_flushStartThreshold)
	{
		s_minimumLow = count;
		s_flushState = STATE_FLUSHING;
		resetFlushDuration();
		if (s_pFnStart) { s_pFnStart(); }
	}
	else
	{
		updateStartThreshold(count);    
	}
}

void LatrineSensor::handleFlushing(uint16_t count)
{
	updateEndThreshold(count);
	updateFlushDuration();
	if (count > s_flushEndThreshold)
	{
		if (s_pFnEnd) { s_pFnEnd( getFlushDurationInSeconds() ); }
		s_flushState = STATE_IDLE;
	}
}

void LatrineSensor::resetFlushDuration(void)
{
	s_flushDurationMs = 0UL;
}

void LatrineSensor::updateFlushDuration(void)
{
	s_flushDurationMs += UPDATE_TICK_MS;
}

uint16_t LatrineSensor::getFlushDurationInSeconds(void)
{
	return (s_flushDurationMs + 500) / 1000;
}


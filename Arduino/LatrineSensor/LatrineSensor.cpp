/*
 * Includes
 */
 
#include <LatrineSensor.h>
#include <util/atomic.h>

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
	/* Timer1 (16-bit timer) setup) 
	- Counts on T1 pin, which is PD5 which is Arduino pin 5 */
	TCCR1A = 0; // Output compare off, waveform generation off
	TCCR1B = 0x06; // Input capture off, waveform generation off, clock source external falling edge
	TCNT1 = 0;
}

uint16_t LatrineSensor::Update(void)
{
	return updateTask();
}

void LatrineSensor::emitDebugInfo(uint16_t lastCount)
{
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
}

uint16_t LatrineSensor::updateTask(void)
{
	volatile uint16_t count;

	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		count = TCNT1;
		TCNT1 = 0;
	}

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

void LatrineSensor::updateCalibration(uint16_t lastCount)
{
	static uint16_t calibCount = 0;

	s_calibCount += lastCount;

	if (++calibCount == 5)
	{
		s_averageHigh = (s_calibCount / 5);
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
		if (s_pFnStart) { s_pFnEnd( getFlushDurationInSeconds() ); }
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


/*
 * Includes
 */
 
#include <LatrineSensor.h>

/*
 * Constants (might become private class variables?
 */

static const uint16_t MINIMUM_FREQ_DIFF = 200;
static const unsigned long UPDATE_TICK_MS = 1000;

// Keep outside class: counts interrupts on sensor pin
static uint32_t s_intCount = 0UL;

static void countISR(void);

LatrineSensor::LatrineSensor(uint8_t pin, LS_START_CB pFnStart, LS_END_CB pFnEnd, bool bEmitDebugInfo)
{
	attachInterrupt(pin, countISR, RISING);
	
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
	
	s_LastUpdate = millis();
}

void LatrineSensor::Update(void)
{
	if (millis() - s_LastUpdate > UPDATE_TICK_MS)
	{
		updateTask();
	}
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

void LatrineSensor::updateTask(void)
{
	uint16_t capturedCount = s_intCount;
	s_intCount = 0;

	switch(s_flushState)
	{
	case STATE_CALIBRATING:
		updateCalibration(capturedCount);
		break;
	case STATE_IDLE:
		handleIdleFlush(capturedCount);
		break;
	case STATE_FLUSHING:
		handleFlushing(capturedCount);
		break;
	}
	
	if (s_bEmitDebugInfo) { emitDebugInfo(capturedCount); }
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

void countISR(void)
{
	s_intCount++;  
}












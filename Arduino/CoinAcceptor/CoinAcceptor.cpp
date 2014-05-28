/*
 * Includes
 */
 
#include <CoinAcceptor.h>

/*
 * File scope functions
 */

static PULSEVALUEMAP * FindEmptyMapPointer(PULSEVALUEMAP * map);
static PULSEVALUEMAP * FindMapForPulses(uint8_t pulses, PULSEVALUEMAP * map);

static void pulseISR(void);

/*
 * File scope variables 
 */

// Keep outside class: counts interrupts on pulse pin
static uint8_t s_intCount = 0UL;
static bool s_newPulse = true;

CoinAcceptor::CoinAcceptor(uint8_t pin, uint16_t mspulse, COIN_INSERTED pfnOnNewCoin)
{
	attachInterrupt(pin, pulseISR, RISING);
	
	s_pfnOnNewCoin = pfnOnNewCoin;
	s_mspulse = mspulse * 2;
	s_LastUpdate = millis();
	
	s_state = IDLE;
	memset(s_pulsestovalues, sizeof(s_pulsestovalues), 0);
}

void CoinAcceptor::AddValue(uint8_t pulses, uint16_t value)
{
	if (pulses > 0)
	{
		PULSEVALUEMAP * pEmptyLocation = FindEmptyMapPointer(s_pulsestovalues);
		
		if (pEmptyLocation)
		{
			pEmptyLocation->pulses = pulses;
			pEmptyLocation->value = value;
		}
	}
}

void CoinAcceptor::Update(void)
{ 
	if ((millis() - s_LastUpdate) > s_mspulse)
	{
		s_LastUpdate = millis();
		updateTask();
	}
}

void CoinAcceptor::updateTask(void)
{
	switch(s_state)
	{
	case IDLE:
		handleIdle();
		break;
	case COUNTING:
		handleCounting();
		break;
	}
}

void CoinAcceptor::handleIdle(void)
{
	s_state = s_intCount ? COUNTING : IDLE;
}

void CoinAcceptor::handleCounting(void)
{
	if (s_newPulse)
	{
		s_newPulse = false;
	}
	else
	{
		handlePulsingStopped();
	}
}

void CoinAcceptor::handlePulsingStopped(void)
{
	uint16_t value = 0;
	PULSEVALUEMAP * map = FindMapForPulses(s_intCount, s_pulsestovalues);
	
	if (map) { value = map->value; }
	
	if (s_pfnOnNewCoin)
	{
		s_pfnOnNewCoin(value, s_intCount);
	}
	
	s_intCount = 0;
	s_state = IDLE;
}

static void pulseISR(void)
{
	s_newPulse = true;
	s_intCount++;
}

static PULSEVALUEMAP * FindEmptyMapPointer(PULSEVALUEMAP * map)
{
	return FindMapForPulses(0, map);
}

static PULSEVALUEMAP * FindMapForPulses(uint8_t pulses, PULSEVALUEMAP * map)
{
	PULSEVALUEMAP * found = NULL;
	
	for (char i = 0; i < 6; i++)
	{
		if (map[i].pulses == pulses)
		{
			found = map;
			break;
		}
	}
	return found;
}

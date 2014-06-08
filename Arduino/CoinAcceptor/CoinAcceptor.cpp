/*
 * Includes
 */
 
#include <CoinAcceptor.h>

/*
 * File scope functions
 */

static void pulseISR(void);

/*
 * File scope variables 
 */

// Keep outside class: counts interrupts on pulse pin
static uint8_t s_intCount = 0UL;
static bool s_newPulse = true;

CoinAcceptor::CoinAcceptor(uint8_t intNumber, uint16_t valuePerPulse, COIN_INSERTED pfnOnNewCoin) :
	updateTask(NULL, 500, INFINITE_TICKS),
	fakeTask(NULL, 100, INFINITE_TICKS)
{
	attachInterrupt(intNumber, pulseISR, RISING);
	
	s_pfnOnNewCoin = pfnOnNewCoin;
	s_LastUpdate = millis();
	
	s_valuePerPulse = valuePerPulse;

	s_fakePin = -1;
	s_fakeCount = 0;
	s_fakeState = LOW;
	
	s_state = IDLE;
}

void CoinAcceptor::Update(void)
{ 
	if (updateTask.tick(0)) { onUpdate(); }
	if (fakeTask.tick(0)) { onFake(); }
}

void CoinAcceptor::Fake(uint8_t count, uint8_t pin)
{
	pinMode(pin, OUTPUT);
	s_fakePin = pin;
	s_fakeCount = count * 2;
}

void CoinAcceptor::onFake(void)
{
	if (s_fakeCount && (s_fakePin > -1))
	{
		digitalWrite(s_fakePin, s_fakeState = !s_fakeState);
		s_fakeCount--;
	}
}

void CoinAcceptor::onUpdate(void)
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
	uint16_t intCount = s_intCount;
	
	/* Reset state and counter so that counting can start again */
	s_intCount = 0;
	s_state = IDLE;
	
	if (s_pfnOnNewCoin)
	{
		s_pfnOnNewCoin(intCount * s_valuePerPulse);
	}
}

void pulseISR(void)
{	
	s_newPulse = true;
	s_intCount++;
}

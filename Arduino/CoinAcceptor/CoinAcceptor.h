#ifndef _COINACCEPTOR_H_
#define _COINACCEPTOR_H_

#include <stdint.h>
#include <Arduino.h>
#include <TaskAction.h>

typedef void(*COIN_INSERTED)(uint16_t value);

class CoinAcceptor
{
	
	public:
		CoinAcceptor(uint8_t intNumber, uint16_t valuePerPulse, COIN_INSERTED pfnOnNewCoin);
		void Update(void);
		void Fake(uint8_t count, uint8_t pin);
	private:
		
		enum acceptor_state
		{
			IDLE,
			COUNTING
		};
	
		void onUpdate(void);
		void onFake(void);
		void handleIdle(void);
		void handleCounting(void);
		void handlePulsingStopped(void);
		
		enum acceptor_state s_state;
		uint16_t s_valuePerPulse;
		COIN_INSERTED s_pfnOnNewCoin;
		uint32_t s_LastUpdate;
		
		uint8_t s_fakeCount;
		int s_fakeState;
		int s_fakePin;

		TaskAction updateTask;
		TaskAction fakeTask;
};

#endif

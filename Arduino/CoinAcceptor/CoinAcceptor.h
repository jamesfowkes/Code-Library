#ifndef _COINACCEPTOR_H_
#define _COINACCEPTOR_H_

#include <stdint.h>
#include <Arduino.h>

typedef void(*COIN_INSERTED)(uint16_t value, uint8_t pulses);

enum AcceptorState
{
	IDLE,
	COUNTING
};

struct pulsevaluemap
{
	uint8_t pulses;
	uint16_t value;
};
typedef struct pulsevaluemap PULSEVALUEMAP;
	
class CoinAcceptor
{
	
	public:
		CoinAcceptor(uint8_t pin, uint16_t mspulse, COIN_INSERTED pfnOnNewCoin);
		void AddValue(uint8_t pulses, uint16_t value);
		void Update(void);
		
	private:
		
		void updateTask(void);
		void handleIdle(void);
		void handleCounting(void);
		void handlePulsingStopped(void);
		
		enum AcceptorState s_state;
		
		COIN_INSERTED s_pfnOnNewCoin;
		uint16_t s_mspulse;
		uint32_t s_LastUpdate;
		struct pulsevaluemap s_pulsestovalues[6];
};

#endif

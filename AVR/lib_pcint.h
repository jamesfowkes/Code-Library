#ifndef _LIB_PCINT_H_
#define _LIB_PCINT_H_

typedef bool PCINT_FLAG;

enum pcint_vector_enum
{
	ePCINT0,
	ePCINT1,
	ePCINT2,
	ePCINT3,
	ePCINT_MAX
};
typedef enum pcint_vector_enum PCINT_VECTOR_ENUM;

void PCINT_EnableInterrupt(uint8_t pin, bool enable);
bool PCINT_TestAndClear(PCINT_VECTOR_ENUM eVector);

#endif
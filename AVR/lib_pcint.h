#ifndef _LIB_PCINT_H_
#define _LIB_PCINT_H_

#if defined(GIMSK)
#define enablePCInterrupts(mask)		(GIMSK |= (mask))
#define disablePCInterrupts(mask)		(GIMSK &= (~(mask)))
#endif

#if defined(PCICR)
#define enablePCInterrupts(mask)		(PCICR |= (1 << PCIE))
#define disablePCInterrupts(mask)		(PCICR &= (~(1 << PCIE)))
#endif

#ifndef enablePCInterrupts
#error "enablePCInterrupts was not defined!"
#endif

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

PCINT_VECTOR_ENUM PCINT_EnableInterrupt(uint8_t pin, bool enable);
bool PCINT_TestAndClear(PCINT_VECTOR_ENUM eVector);

#endif

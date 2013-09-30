#ifndef _LIB_PCINT_H_
#define _LIB_PCINT_H_

typedef bool PCINT_FLAG;

enum pcint_enum
{
	eINT0,
	eINT1,
	eINT2,
	eINT3,
	eINT4,
	eINT5,
	eINT6,
	eINT7,
	ePCINT0,
	ePCINT1,
	ePCINT2,
	ePCINT3,
	ePCINT4,
	ePCINT5,
	ePCINT6,
	ePCINT7,
	ePCINT_MAX
};
typedef enum pcint_enum PCINT_ENUM;

bool PCINT_TestAndClear(PCINT_ENUM eInterrupt);

#endif
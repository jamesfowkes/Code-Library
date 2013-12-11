#ifndef _LIB_EXTINT_H_
#define _LIB_EXTINT_H_

typedef bool EXTINT_FLAG;

enum extint_vector_enum
{
	eEXTINT0,
	eEXTINT1,
	eEXTINT_MAX
};
typedef enum extint_vector_enum EXTINT_VECTOR_ENUM;

enum extint_sense_enum
{
	LOW,
	CHANGE,
	FALLING,
	RISING
};
typedef enum extint_sense_enum EXTINT_SENSE_ENUM;

void EXTINT_SetSensitivity(EXTINT_VECTOR_ENUM eVector, EXTINT_SENSE_ENUM sensitivity);
void EXTINT_EnableInterrupt(EXTINT_VECTOR_ENUM eVector, bool enable);
bool EXTCINT_TestAndClear(EXTINT_VECTOR_ENUM eVector);

#endif

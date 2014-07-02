#ifndef _LIB_8BITTMR_H_
#define _LIB_8BITTMR_H_

#include "lib_io.h"
#include "lib_tmr_common.h"

/*
 * Public Datatypes
 */

enum tmr8_countmode_enum
{
    TMR8_COUNTMODE_NORMAL,
	TMR8_COUNTMODE_PCPWM1,		// TOP = 0xFF
	TMR8_COUNTMODE_PCPWM2,		// TOP = OCR0A
    TMR8_COUNTMODE_CTC,
    TMR8_COUNTMODE_FASTPWM1,	// TOP = 0xFF
	TMR8_COUNTMODE_FASTPWM2,	// TOP = OCR0A
	TMR8_COUNTMODE_INVALID,
};
typedef enum tmr8_countmode_enum TMR8_COUNTMODE_ENUM;

enum tmr8_intmask_enum
{
	#ifdef OCIE0B
	TMR8_INTMASK_OCMPB	= (1 << OCIE0B),
	#endif
	
	#ifdef OCIE0A
	TMR8_INTMASK_OCMPA	= (1 << OCIE0A),
	#endif
	
	#ifdef OCIE0
	TMR8_INTMASK_OCMPA	= (1 << OCIE0),
	#endif
	
	TMR8_INTMASK_OVF	= (1 << TOIE0)
};
typedef enum tmr8_intmask_enum TMR8_INTMASK_ENUM;

/*
 * Public Function Prototypes
 */

void TMR8_SetSource(TMR_SRC_ENUM eSource);
TMR_SRC_ENUM TMR8_GetSource(void);
uint16_t TMR8_GetDivisor(void);

void 					TMR8_SetCountMode(const TMR8_COUNTMODE_ENUM eMode);
TMR8_COUNTMODE_ENUM		TMR8_GetCountMode(void);

void					TMR8_SetOutputCompareMode(const TMR_OUTPUTMODE_ENUM eOutputMode, const TMR_OCCHAN_ENUM eChannel);
TMR_OUTPUTMODE_ENUM		TMR8_GetOutputCompareMode(const TMR_OCCHAN_ENUM eChannel);
void					TMR8_SetOutputCompareValue(const uint8_t value, const TMR_OCCHAN_ENUM eChannel);

void					TMR8_ForceOutputCompare(const TMR_OCCHAN_ENUM eChannel);
void					TMR8_PWMOff(const TMR_OCCHAN_ENUM eChannel, const IO_STATE_ENUM eState);

void					TMR8_InterruptControl(TMR8_INTMASK_ENUM eMask, bool enable);

#endif

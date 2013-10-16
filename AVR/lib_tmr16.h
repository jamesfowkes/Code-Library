#ifndef _LIB_16BITTMR_H_
#define _LIB_16BITTMR_H_

#include "lib_tmr_common.h"

/*
 * Public Datatypes
 */

enum tmr16_countmode_enum
{
	TMR16_COUNTMODE_NORMAL,
	TMR16_COUNTMODE_PCPWM_8BIT, 	// TOP = 0xFF
	TMR16_COUNTMODE_PCPWM_9BIT, 	// TOP = 0x1FF
	TMR16_COUNTMODE_PCPWM_10BIT, 	// TOP = 0x3FF
	TMR16_COUNTMODE_CTC,
	TMR16_COUNTMODE_FASTPWM_8BIT, 	// TOP = 0xFF
	TMR16_COUNTMODE_FASTPWM_9BIT, 	// TOP = 0x1FF
	TMR16_COUNTMODE_FASTPWM_10BIT, 	// TOP = 0x3FF
	TMR16_COUNTMODE_PC_FC_1,		// TOP = ICR1
	TMR16_COUNTMODE_PC_FC_2,		// TOP = OCR1A
	TMR16_COUNTMODE_PC_1,		// TOP = ICR1
	TMR16_COUNTMODE_PC_2,		// TOP = OCR0A
	TMR16_COUNTMODE_INVALID,
	TMR16_COUNTMODE_FASTPWM_1,	// TOP = ICR1
	TMR16_COUNTMODE_FASTPWM_2,	// TOP = OCR0A
	TMR16_COUNTMODE_MAX
};
typedef enum tmr16_countmode_enum TMR16_COUNTMODE_ENUM;

enum tmr16_intmask_enum
{
	TMR16_INTMASK_IC	= 0x20,
	TMR16_INTMASK_OCMPC	= 0x08,
	TMR16_INTMASK_OCMPB	= 0x04,
	TMR16_INTMASK_OCMPA	= 0x02,
	TMR16_INTMASK_OVF	= 0x01
};
typedef enum tmr16_intmask_enum TMR16_INTMASK_ENUM;

/*
 * Public Function Prototypes
 */

void TMR16_SetSource(TMR_SRC_ENUM eSource);
TMR_SRC_ENUM TMR16_GetSource(void);
uint16_t TMR16_GetDivisor(void);

void 					TMR16_SetCountMode(const TMR16_COUNTMODE_ENUM eMode);

void					TMR16_SetOutputCompareMode(const TMR_OUTPUTMODE_ENUM eOutputMode, const TMR_OCCHAN_ENUM eChannel);
TMR_OUTPUTMODE_ENUM		TMR16_GetOutputCompareMode(const TMR_OCCHAN_ENUM eChannel);
void					TMR16_SetOutputCompareValue(const uint16_t value, const TMR_OCCHAN_ENUM eChannel);

void					TMR16_ForceOutputCompare(const TMR_OCCHAN_ENUM eChannel);
void					TMR16_PWMOff(const TMR_OCCHAN_ENUM eChannel, const IO_STATE_ENUM eState);

void					TMR16_InterruptControl(TMR16_INTMASK_ENUM eMask, bool enable);

bool					TMR16_StartTimer(uint16_t us, TIMER_FLAG * timerFlag, const TMR_OCCHAN_ENUM eChannel);
void 					TMR16_StopTimer(const TMR_OCCHAN_ENUM eChannel);
#endif

#ifndef _LIB_8BITTMR_H_
#define _LIB_8BITTMR_H_

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

enum tmr8_occhan_enum
{
	TMR8_OCCHAN_A,
	TMR8_OCCHAN_B,
	TMR8_OCCHAN_INVALID,
};
typedef enum tmr8_occhan_enum TMR8_OCCHAN_ENUM;

enum tmr8_outputmode_enum
{
	TMR8_OUTPUTMODE_NONE,
	TMR8_OUTPUTMODE_TOGGLE,
	TMR8_OUTPUTMODE_CLEAR,
	TMR8_OUTPUTMODE_SET,
	TMR8_OUTPUTMODE_INVALID
};
typedef enum tmr8_outputmode_enum TMR8_OUTPUTMODE_ENUM;

typedef void (*tmr8_cb)(uint32_t seconds);

struct tmr8_tick_config
{
	LINK_NODE Node;
	TMR8_OCCHAN_ENUM eChannel;
	tmr8_cb Callback;
	uint32_t msTick;
};
typedef struct tmr8_tick_config TMR8_TICK_CONFIG;

/*
 * Public Function Prototypes
 */
void TMR8_Init(void);

bool TMR8_AddCallbackTick(TMR8_TICK_CONFIG * config);
void TMR8_SetPWMDuty(uint8_t duty);
void TMR8_SelectSource(TMR_SRC_ENUM eSource);

void 					TMR8_SetCountMode(const TMR8_COUNTMODE_ENUM eMode);
TMR8_COUNTMODE_ENUM		TMR8_GetCountMode(void);

void					TMR8_SetOutputCompareMode(const TMR8_OUTPUTMODE_ENUM eOutputMode, const TMR8_OCCHAN_ENUM eChannel);
TMR8_OUTPUTMODE_ENUM	TMR8_GetOutputCompareMode(const TMR8_OCCHAN_ENUM eChannel);
void					TMR8_SetOutputCompareValue(const uint8_t value, const TMR8_OCCHAN_ENUM eChannel);

void					TMR8_ForceOutputCompare(const TMR8_OCCHAN_ENUM eChannel);

#endif
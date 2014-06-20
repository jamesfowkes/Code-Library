#ifndef _LIB_16BITTMR_PWM_H_
#define _LIB_16BITTMR_PWM_H_

#include "lib_tmr_common.h"

/*
 * Public Datatypes
 */

#ifdef TEST_HARNESS
struct tmr16_pwm_debug
{
	uint16_t prescaler;
	uint16_t ocr;
	uint16_t top;
};
typedef struct tmr16_pwm_debug TMR16_PWM_DEBUG;
#endif

/*
 * Public Function Prototypes
 */

#ifdef TEST_HARNESS
bool TMR16_PWM_Set(uint16_t freq, uint16_t duty, TMR_OCCHAN_ENUM eChannel, TMR16_PWM_DEBUG * pData);
#else
bool TMR16_PWM_Set(uint16_t freq, uint16_t duty, TMR_OCCHAN_ENUM eChannel);
#endif

#endif

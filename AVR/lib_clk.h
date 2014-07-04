#ifndef _LIB_CLK_H_
#define _LIB_CLK_H_

/*
 * Requirements for this module
 */

#include <avr/power.h>

// Some AVR devices do not have a prescaler, so clock_div_t is not a type
#ifdef clock_prescale_get

#define AVR_HAS_PRESCALER

#else

#ifndef TEST_HARNESS

typedef enum
{
    clock_div_1 = 0,
} clock_div_t;

#endif

#endif

enum lib_clk_src_enum
{
	LIB_CLK_SRC_RC,
	LIB_CLK_SRC_EXT
};
typedef enum lib_clk_src_enum LIB_CLK_SRC_ENUM;

bool		CLK_Init(const uint32_t fExtSet);
void		CLK_SetSource(const LIB_CLK_SRC_ENUM eSetSource);
void		CLK_SetPrescaler(const clock_div_t eSetPrescaler);
clock_div_t	CLK_GetPrescaler(void);
uint32_t	CLK_GetFcpu(void);

bool		CLK_IsSourceRunning(const LIB_CLK_SRC_ENUM eSource);

#endif

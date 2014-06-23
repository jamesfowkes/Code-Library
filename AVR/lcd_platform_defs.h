#ifndef _LCD_PLATFORM_DEFS_H_
#define _LCD_PLATFORM_DEFS_H_

#include <util/delay.h>

#define lcd_e_delay()   __asm__ __volatile__( "rjmp 1f\n 1:" );

/************************************************************************* 
delay for a minimum of <us> microseconds
the number of loops is calculated at compile-time from MCU clock frequency
*************************************************************************/
#define delay(us) _delay_us(us)

#endif
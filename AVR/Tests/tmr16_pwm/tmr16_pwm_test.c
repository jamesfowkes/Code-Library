/*
 * Standard Library Includes
 */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include <avr/io.h>

#include "../../lib_io.h"
#include "../../lib_clk.h"
#include "../../lib_tmr16_pwm.h"
#include "../../../Utility/util_macros.h"

TMR16_PWM_DEBUG debug;

static bool matchFreq(uint32_t f1, uint32_t f2)
{
	if (f1 > 1000)
	{
		return abs(f1 - f2) < 10;
	}
	else
	{
		return (f1 == f2);
	}
}

static bool matchDuty(uint32_t d1, uint32_t d2)
{
	return abs(d1 - d2) < 2;
}

int main(int argc, char *argv[])
{
	(void)argc;
	(void)argv;
	
	setbuf(stdout, NULL);
	
	uint16_t freq = 5;
	uint8_t duty = 1;
	const uint32_t fcpu = CLK_GetFcpu();
	
	printf("FCPU = %d\n", fcpu);
	
	for (freq = 5; freq < 5000; freq++)
	{
		for (duty = 1; duty < 101; duty++)
		{
			TMR16_PWM_Set(freq, duty, TMR_OCCHAN_A, &debug);
			
			// Calculate frequency with these settings
			uint32_t tmrClockFreq = fcpu / debug.prescaler;
			uint32_t tmrOverflowFreq = div_round_pos(tmrClockFreq, debug.top);
			uint32_t tmrDuty = (debug.ocr * 100) / debug.top;
			
			if (!matchFreq(tmrOverflowFreq, freq))
			{
				printf("Got freq %d, expected %d for pre = %d, top = %d\n", tmrOverflowFreq, freq, debug.prescaler, debug.top);
			}
			
			if (!matchDuty(tmrDuty, duty))
			{
				printf("Got duty %d, expected %d for pre = %d, ocr = %d\n", tmrDuty, duty, debug.prescaler, debug.ocr);
			}
		}
	}
	
	return 0;
}

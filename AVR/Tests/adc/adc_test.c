/*
 * Standard Library Includes
 */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include <pthread.h>
#include "../../lib_adc.h"

static ADC_CONTROL_ENUM adc;

int main(int argc, char *argv[])
{
	(void)argc;
	(void)argv;
	
	setbuf(stdout, NULL);
	
	ADC_SelectPrescaler(LIB_ADC_PRESCALER_DIV64);
	ADC_SelectReference(LIB_ADC_REF_VCC);
	ADC_Enable(true);
	ADC_EnableInterrupts(true);

	adc.busy = false;
	adc.channel = LIB_ADC_CH_0;
	adc.conversionComplete = false;

	ADC_GetReading(&adc);
	
	while(true)
	{
		if (ADC_TestAndClear(&adc))
		{
			printf("Got reading %u\n", 0);
			break;
		}
	}
	
	return 0;
}

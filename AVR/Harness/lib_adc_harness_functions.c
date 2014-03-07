/*
 * Standard Library Includes
 */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>

#include "lib_adc.h"
#include "lib_adc_harness_functions.h"

static char * arrayPtrs[LIB_ADC_CH_MAX];
static uint16_t readIndexes[LIB_ADC_CH_MAX];
static uint16_t arrayLengths[LIB_ADC_CH_MAX];

void ADC_Harness_SetReadingArray(LIB_ADC_CHANNEL_ENUM eChannel, uint16_t * array, uint16_t length)
{
	arrayPtrs[eChannel] = array;
	readIndexes[eChannel] = 0;
	arrayLengths[eChannel] = length;
}

void ADC_Harness_TriggerISR(ADC_CONTROL_ENUM * adc)
{
	adc->reading = arrayPtrs[adc->channel][ readIndexes[adc->channel] ];
	adc->conversionComplete = true;
	adc->busy = false;
	
	incrementwithrollover(readIndexes[adc->channel], arrayLengths[LIB_ADC_CH_MAX] - 1);

}
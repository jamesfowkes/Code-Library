/*
 * Standard Library Includes
 */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>

#include "util_sequence_generator.h"
#include "lib_adc.h"
#include "lib_adc_harness_functions.h"

static SEQUENCE * seqPtrs[LIB_ADC_CH_MAX];

void ADC_Harness_SetReadingArray(LIB_ADC_CHANNEL_ENUM eChannel, SEQUENCE * seq)
{
	seqPtrs[eChannel] = seq;
}

void ADC_Harness_TriggerISR(ADC_CONTROL_ENUM * adc)
{
	adc->reading = SEQGEN_Read ( seqPtrs[adc->channel] );
	adc->conversionComplete = true;
	adc->busy = false;
}

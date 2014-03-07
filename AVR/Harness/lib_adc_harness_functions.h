#ifndef _LIB_ADC_HARNESS_H_
#define _LIB_ADC_HARNESS_H_

void ADC_Harness_TriggerISR(ADC_CONTROL_ENUM * adc);
void ADC_Harness_SetReadingArray(LIB_ADC_CHANNEL_ENUM eChannel, uint16_t * array, uint16_t length);

#endif

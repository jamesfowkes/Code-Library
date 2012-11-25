#ifndef _LIB_ADC_H_
#define _LIB_ADC_H_

/*
 * Public Datatype Definitions
 */

enum lib_adc_channel_enum
{
	LIB_ADC_CH_0,
	LIB_ADC_CH_1,
	LIB_ADC_CH_2,
	LIB_ADC_CH_3,
	LIB_ADC_CH_4,
	LIB_ADC_CH_5,
	LIB_ADC_CH_6,
	LIB_ADC_CH_7,
	LIB_ADC_CH_MAX
};
typedef enum lib_adc_channel_enum LIB_ADC_CHANNEL_ENUM;

enum lib_adc_reference_enum
{
	LIB_ADC_REF_VCC,
	LIB_ADC_REF_EXT,
	LIB_ADC_REF_INT,
};
typedef enum lib_adc_reference_enum LIB_ADC_REFERENCE_ENUM;

enum lib_adc_prescaler_enum
{
	LIB_ADC_PRESCALER_DIV2,
	LIB_ADC_PRESCALER_DIV4,
	LIB_ADC_PRESCALER_DIV8,
	LIB_ADC_PRESCALER_DIV16,
	LIB_ADC_PRESCALER_DIV32,
	LIB_ADC_PRESCALER_DIV64,
	LIB_ADC_PRESCALER_DIV128,
};
typedef enum lib_adc_prescaler_enum LIB_ADC_PRESCALER_ENUM;

void ADC_Enable(bool enableADC);
void ADC_EnableInterrupts(bool enableInterrupts);
void ADC_AutoTriggerEnable(bool enableAutoTrigger);

uint16_t ADC_GetReading(LIB_ADC_CHANNEL_ENUM eChannel);

void ADC_GetLastReading(uint16_t *reading, LIB_ADC_CHANNEL_ENUM *eChannel);

void ADC_SelectReference(LIB_ADC_REFERENCE_ENUM eRef);
void ADC_SelectPrescaler(LIB_ADC_PRESCALER_ENUM ePrescaler);
#endif

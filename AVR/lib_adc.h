#ifndef _LIB_ADC_H_
#define _LIB_ADC_H_

/*
 * Public Defines
 */

#define MAX_8BIT_ADC ((1 << 8) - 1)
#define MAX_9BIT_ADC ((1 << 9) - 1)
#define MAX_10BIT_ADC ((1 << 10) - 1)

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
	LIB_ADC_PRESCALER_DIV2 = 1,
	LIB_ADC_PRESCALER_DIV4,
	LIB_ADC_PRESCALER_DIV8,
	LIB_ADC_PRESCALER_DIV16,
	LIB_ADC_PRESCALER_DIV32,
	LIB_ADC_PRESCALER_DIV64,
	LIB_ADC_PRESCALER_DIV128,
};
typedef enum lib_adc_prescaler_enum LIB_ADC_PRESCALER_ENUM;

struct adc_control_enum
{
	LIB_ADC_CHANNEL_ENUM channel;
	uint16_t reading;
	bool conversionComplete;
	bool busy;
};
typedef struct adc_control_enum ADC_CONTROL_ENUM;

void ADC_Enable(bool enableADC);
void ADC_EnableInterrupts(bool enableInterrupts);
void ADC_AutoTriggerEnable(bool enableAutoTrigger);

void ADC_GetReading(ADC_CONTROL_ENUM * control);

void ADC_GetLastReading(uint16_t *reading, LIB_ADC_CHANNEL_ENUM *eChannel);

void ADC_SelectReference(LIB_ADC_REFERENCE_ENUM eRef);
void ADC_SelectPrescaler(LIB_ADC_PRESCALER_ENUM ePrescaler);

bool ADC_TestAndClear(ADC_CONTROL_ENUM * control);

#endif

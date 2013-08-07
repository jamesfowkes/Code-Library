/*
 * Standard Library Includes
 */
#include <stdlib.h>
#include <stdbool.h>

/*
 * Defines
 */

#define CHANNEL_MASK ((1 << MUX5) | (1 << MUX4) | (1 << MUX3) | (1 << MUX2) | (1 << MUX1)  | (1 << MUX0))

/*
 * AVR Includes (Defines and Primitives)
 */
#include "avr/io.h"
#include <avr/interrupt.h>

/*
 * AVR Library Include
 */

#include "lib_adc.h"
#include "lib_io.h"

/*
 * Private Variables
 */

static volatile bool validReading = false;

static LIB_ADC_CHANNEL_ENUM currentChannel;
static volatile uint16_t lastReading;

static volatile ADC_CONTROL_ENUM *pControl = NULL;

/*
 * Private Function Prototypes
 */

static void SetChannel(LIB_ADC_CHANNEL_ENUM eChannel);

/*
 * Public Functions
 */

void ADC_Enable(bool enableADC)
{
	uint8_t adcsra = ADCSRA;

	if (enableADC)
	{
		adcsra |= (1 << ADEN);
	}
	else
	{
		adcsra &= ~(1 << ADEN);
	}

	ADCSRA = adcsra;
}

void ADC_EnableInterrupts(bool enableInterrupts)
{
	uint8_t adcsra = ADCSRA;

	if (enableInterrupts)
	{
		adcsra |= (1 << ADIE);
	}
	else
	{
		adcsra &= ~(1 << ADIE);
	}

	ADCSRA = adcsra;
}

#ifdef LIB_ADC_USE_AUTOTRIGGER
void ADC_AutoTriggerEnable(bool enableAutoTrigger)
{
	uint8_t adcsra = ADCSRA;

	if (enableAutoTrigger)
	{
		adcsra |= (1 << ADATE);
	}
	else
	{
		adcsra &= ~(1 << ADATE);
	}

	ADCSRA = adcsra;
}
#endif

uint16_t ADC_GetReading(ADC_CONTROL_ENUM * control)
{
	control->validReading = false;
	SetChannel(control->channel);
	pControl = control;
	
	ADCSRA |= (1 << ADSC); // Start conversion
}

void ADC_SelectReference(LIB_ADC_REFERENCE_ENUM eRef)
{
	uint8_t admux = ADMUX;

	admux &= ~((1 << REFS1) | (1 << REFS0));

	admux |= (uint8_t)eRef << REFS0;

	ADMUX = admux;
}

void ADC_SelectPrescaler(LIB_ADC_PRESCALER_ENUM ePrescaler)
{
	uint8_t adcsra = ADCSRA;

	adcsra &= ~((1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0));

	adcsra |= (uint8_t)ePrescaler & ((1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0));

	ADCSRA = adcsra;
}

void ADC_GetLastReading(uint16_t *reading, LIB_ADC_CHANNEL_ENUM *eChannel)
{
	*eChannel = currentChannel;
	*reading = lastReading;
}

/*
 * Private Functions
 */

static void SetChannel(LIB_ADC_CHANNEL_ENUM eChannel)
{
	uint8_t admux = ADMUX;

	currentChannel = eChannel;

	admux &= ~CHANNEL_MASK;

	admux |= (uint8_t)eChannel & CHANNEL_MASK;

	ADMUX = admux;
}

ISR(ADC_vect)
{
	uint16_t result = 0;

	result = ADCL;
	result |= ADCH << 8;

	lastReading = result;
	pControl->reading = lastReading;
	pControl->validReading = true;
}

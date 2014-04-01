/*
 * Standard Library Includes
 */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>

/*
 * AVR Includes (Defines and Primitives)
 */
#include <avr/io.h>
#include <avr/interrupt.h>

/*
 * AVR Library Includes
 */

#include "lib_usi.h"

/*
 * Private Function Prototypes
 */
static uint8_t getClockSourceSetting(USI_CLK_SRC_ENUM clockSource);
static uint8_t getCounterSourceSetting(USI_COUNT_SRC_ENUM counterSource);

/*
 * Private Variables
 */
static USI_DATA * pData;

uint8_t USI_SendByte(uint8_t byte)
{
	USIDR = byte;
	while (((USISR & (1 << USIOIF)) == 0))
	{
		USICR |= (1 << USITC);
	}
	return USIDR;
}

void USI_SetReply(uint8_t byte, USI_DATA * data)
{
	pData = data;
	USIDR = byte;
}

void USI_SetMode(USI_MODE_ENUM mode)
{
	// Set USI mode
	uint8_t usicr = USICR;
	usicr &= ~((1 << USIWM1) | (1 << USIWM0));
	usicr |= ((uint8_t)mode << USIWM0);

	USICR = usicr;
}

void USI_StartInterruptEnable(bool enable)
{
	uint8_t usicr = USICR;

	if (enable)
	{
		usicr |= (1 << USISIE);
	}
	else
	{
		usicr &= ~(1 << USISIE);
	}

	USICR = usicr;
}

void USI_CounterInterruptEnable(bool enable)
{
	uint8_t usicr = USICR;

	if (enable)
	{
		usicr |= (1 << USIOIE);
	}
	else
	{
		usicr &= ~(1 << USIOIE);
	}

	USICR = usicr;
}

void USI_SetSources(USI_CLK_SRC_ENUM clockSource, USI_COUNT_SRC_ENUM countSource)
{
	bool valid = false;
	uint8_t setting = 0;
	uint8_t usicr = USICR;

	valid = (clockSource == USI_CLK_SRC_NONE) && (countSource == USI_COUNT_SRC_NONE);
	valid |= (clockSource == USI_CLK_SRC_USICLK) && (countSource == USI_COUNT_SRC_USICLK);
	valid |= (clockSource == USI_CLK_SRC_TMR0) && (countSource == USI_COUNT_SRC_TMR0);

	if ((clockSource == USI_CLK_SRC_EXT_POS) || (clockSource == USI_CLK_SRC_EXT_NEG))
	{
		valid = (countSource == USI_COUNT_SRC_USITC) || (countSource == USI_COUNT_SRC_EXT);
	}

	if (valid)
	{
		setting = getClockSourceSetting(clockSource);
		setting |= getCounterSourceSetting(countSource);
		usicr &= ~((1 << USICS1) | (1 << USICS0) | (1 << USICLK));
		usicr |= setting;
		USICR = usicr;
	}
}

bool USI_GetStatusFlag(USI_STATUS_FLAG_ENUM flag)
{
	uint8_t usisr = USISR;
	bool set = false;

	switch (flag)
	{
	case USI_STATUS_START_CONDITION:
		set = ((usisr & (1 << USISIF)) == USISIF);
		break;
	case USI_STATUS_OVERFLOW:
		set = ((usisr & (1 << USIOIF)) == USIOIF);
		break;
	case USI_STATUS_STOP_CONDITION:
		set = ((usisr & (1 << USIPF)) == USIPF);
		break;
	case USI_STATUS_DATA_COLLISION:
		set = ((usisr & (1 << USIDC)) == USIDC);
		break;
	}

	return set;
}

uint8_t USI_GetCounterValue(void)
{
	uint8_t usisr = USISR;

	return usisr & ((1 << USICNT3) | (1 << USICNT2) | (1 << USICNT1) | (1 << USICNT0));
}

/*
 * Private Function Defintions
 */
static uint8_t getClockSourceSetting(USI_CLK_SRC_ENUM clockSource)
{
	uint8_t setting = 0;

	switch (clockSource)
	{
	case USI_CLK_SRC_NONE:
	case USI_CLK_SRC_USICLK:
		break;
	case USI_CLK_SRC_TMR0:
		setting = (1 << USICS0);
		break;
	case USI_CLK_SRC_EXT_POS:
		setting = (1 << USICS1);
		break;
	case USI_CLK_SRC_EXT_NEG:
		setting = (1 << USICS1) | (1 << USICS0);
		break;
	}

	return setting;
}

static uint8_t getCounterSourceSetting(USI_COUNT_SRC_ENUM counterSource)
{
	uint8_t setting = 0;

	if ((counterSource == USI_COUNT_SRC_USICLK) || (counterSource == USI_COUNT_SRC_USITC))
	{
		setting = (1 << USICLK);
	}
	else
	{
		setting = 0;
	}

	return setting;
}

ISR(USI_OVF_vect)
{
	USISR = (1<<USIOIF);

	if (pData)
	{
		pData->byte = USIDR;
		pData->transferComplete = true;
	}
}

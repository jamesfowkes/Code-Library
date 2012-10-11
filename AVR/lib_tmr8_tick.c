/*
 * Standard Library Includes
 */
#include <stdbool.h>
#include <stddef.h>

#include <stdlib.h>
#include <assert.h>

/*
 * Generic Library Includes
 */
#include "linkedlist.h"

/*
 * AVR Includes (Defines and Primitives)
 */
#include <avr/io.h>
#include <avr/power.h>
#include <avr/interrupt.h>

/*
 * AVR Library Includes
 */

#include "lib_clk.h"
#include "lib_tmr8.h"

/*
 * Private Variables
 */

static LINK_NODE * Head;
static uint32_t secondsSinceInit = 0;
static uint16_t msCounter = 0;

/*
 * Private Function Prototypes
 */

bool msListCallback(LINK_NODE * node);

/*
 * Public Functions
 */

void TMR8_Tick_Init(void)
{
	Head = NULL;
	LList_Init(Head);

	TMR8_Init();

	TMR8_SetOutputCompareMode(TMR8_OUTPUTMODE_NONE, TMR8_OCCHAN_B);

	/* Calculate value based on clock frequency and timer division */
	uint8_t cpuDiv = (uint8_t)CLK_GetPrescaler();
	uint8_t tmr8Div = (uint8_t)TMR8_GetSource();

	uint8_t oneMilli = F_CPU/(1 << cpuDiv);	// Clock module input
	oneMilli /= (1 << (tmr8Div - 1));		// Timer 8 prescaler
	oneMilli /= 1000;						// One millisecond

	TMR8_SetOutputCompareValue(oneMilli, TMR8_OCCHAN_B);

	TMR8_InterruptControl(TMR8_INTMASK_OCMPB, true);

	secondsSinceInit = 0;
}

bool TMR8_Tick_AddCallback(TMR8_TICK_CONFIG * config)
{
	bool success = false;

	assert(config->reload > 0);

	if (config->reload > 0)
	{
		config->msTick = config->reload;
		success = LList_Add(Head, &(config->Node));
	}

	return success;
}

bool msListCallback(LINK_NODE * node)
{
	// Safe pointer conversion
	TMR8_TICK_CONFIG * config = (TMR8_TICK_CONFIG*)node;

	config->Callback(secondsSinceInit);

	return false; // continue traversing
}

ISR(TIMER0_COMPB_vect)
{
	if (1000 == ++msCounter)
	{
		++secondsSinceInit;
		msCounter = 0;
	}

	LList_Traverse(Head, msListCallback);

}

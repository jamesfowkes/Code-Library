/*
 * Standard Library Includes
 */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include <stdlib.h>
#include <assert.h>

/*
 * AVR Includes (Defines and Primitives)
 */
#include "avr/io.h"
#include "avr/power.h"
#include "avr/interrupt.h"

/*
 * Generic Library Includes
 */
#include "linkedlist.h"

/*
 * AVR Library Includes
 */
#include "lib_clk.h"

/*
 * Application Includes
 */
#include "minimus.h"

/*
 * Defines
 */

/* IO for Minumus AVR USB key */
#define IO_PRT PORTD
#define IO_DDR DDRD
#define IO_PINS PIND

#define BUTTON_PIN PIND7
#define LED1_PIN	PIND5
#define LED2_PIN	PIND6

/*
 * Private Datatypes
 */
struct button_state
{
	MINIMUS_BUTTONSTATE_ENUM eState;
};
typedef struct button_state BUTTON_STATE;

/*
 * Private Function Prototypes
 */

/*
 * Private Variables
 */
static minimus_button_cb buttonCallback = NULL;
static BUTTON_STATE buttons;

/*
 * Public Functions
 */
void Minimus_Init(minimus_button_cb cb)
{

	buttonCallback = cb;

	buttons.eState = BUTTONUP;

	/* Button to pull-up input, LEDs to output */
	uint8_t temp = 0;

	temp = IO_DDR;
	temp &= ~(1 << BUTTON_PIN);
	temp |= (1 << LED1_PIN);
	temp |= (1 << LED2_PIN);
	IO_DDR = temp;

	temp = IO_PRT;
	temp |= (1 << BUTTON_PIN);
	temp &= ~(1 << LED1_PIN);
	temp &= ~(1 << LED2_PIN);
	IO_PRT = temp;

	CLK_Init(F_USB);
	CLK_SetPrescaler(clock_div_1);
	CLK_SetSource(LIB_CLK_SRC_EXT);
}

void Minimus_IO_Control(MINIMUS_LED_ENUM eLED, MINIMUS_LEDCTRL_ENUM eControl)
{
	assert ((eLED == LED1) || (eLED == LED2));

	uint8_t ledPin = (eLED == LED1) ? LED1_PIN : LED2_PIN;

	switch(eControl)
	{
	case LED_OFF:
		IO_PRT &= ~(1 << ledPin);
		break;
	case LED_ON:
		IO_PRT |= (1 << ledPin);
		break;
	case LED_TOGGLE:
		IO_PINS |= (1 << ledPin);
		break;
	}
}

void Minimus_USB_MsTick(void)
{

}

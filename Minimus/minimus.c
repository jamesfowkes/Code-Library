/*
 * Standard Library Includes
 */
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

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
#include "lib_tmr8.h"
#include "lib_tmr8_tick.h"

/*
 * Application Includes
 */
#include "minimus.h"

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
static void ButtonCheckTick(uint32_t seconds __attribute__ ((unused)) );

/*
 * Private Variables
 */
static minimus_button_cb buttonCallback = NULL;
static BUTTON_STATE buttons[2];

static TMR8_TICK_CONFIG tmr8_tick_config;

/*
 * Public Functions
 */
void Minimus_Init(minimus_button_cb cb)
{

	buttonCallback = cb;

	buttons[BUTTON1].eState = BUTTONUP;
	buttons[BUTTON2].eState = BUTTONUP;

	/* Both buttons to pull-up input */
	uint8_t temp = 0;

	temp = BUTTON1_DDR;
	temp &= ~(BUTTON1_PIN);
	BUTTON1_DDR = temp;

	temp = BUTTON2_PRT;
	temp |= (1 << BUTTON2_PIN);
	BUTTON2_PRT = temp;

	temp = BUTTON2_DDR;
	temp &= ~(BUTTON2_PIN);
	BUTTON2_DDR = temp;

	CLK_Init(F_USB);
	CLK_SetPrescaler(clock_div_1);
	CLK_SetSource(LIB_CLK_SRC_EXT);

	TMR8_SetSource(TMR_SRC_FCLK);

	TMR8_Tick_Init();

	tmr8_tick_config.Callback = ButtonCheckTick;
	tmr8_tick_config.reload = 50;
	TMR8_Tick_AddCallback(&tmr8_tick_config);
}

/*
 * Private Functions
 */
static void ButtonCheckTick(uint32_t seconds __attribute__ ((unused)) )
{

}

/*
 * Standard Library Includes
 */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/*
 * Generic library includes
 */
#include "button.h"

/*
 * Private Function Prototypes
 */

static BTN_STATE_ENUM debounce(BTN * btn, BTN_STATE_ENUM state);
static void onStateChange(BTN * btn, BTN_STATE_ENUM new_state);
static void onSameState(BTN * btn);

/*
 * Register a button btn with this module.
 * The button state should be set to the initial state.
 * The max debounce count should be set to the desired debounce count.
 */
bool BTN_InitHandler(BTN * btn)
{

	bool success = true;
	
	success &= (btn->change_state_callback != NULL);
	success &= (btn->repeat_callback != NULL);
	success &= (btn->max_debounce_count > 0);
	success &= (btn->max_repeat_count > 0);
	
	if (success)
	{
		btn->repeat_count = 0;
		btn->debounce_count = (btn->current_state == BTN_STATE_ACTIVE ? btn->max_debounce_count : 0);
	}
	
	return success;
}

void BTN_Update(BTN * btn, BTN_STATE_ENUM state)
{
	if (btn)
	{
		BTN_STATE_ENUM new_state = debounce(btn, state);
		
		if (new_state != btn->current_state)
		{
			onStateChange(btn, new_state);
		}
		else
		{
			onSameState(btn);
		}
	}
}

/*
 * Updates active button state by running sum debouncing
 */
static BTN_STATE_ENUM debounce(BTN * btn, BTN_STATE_ENUM state)
{

	bool new_state = btn->current_state;
	
	if (state == BTN_STATE_ACTIVE)
	{
		// Increment debounce counter for active state
		btn->debounce_count = (btn->debounce_count < btn->max_debounce_count) ? btn->debounce_count + 1 : btn->max_debounce_count;
	}
	else
	{
		// Increment debounce counter for inactive state
		btn->debounce_count = (btn->debounce_count > 0) ? btn->debounce_count - 1 : 0;
	}
	
	if ((btn->debounce_count == btn->max_debounce_count) || (btn->debounce_count == 0))
	{
		new_state = state;
	}
	
	return new_state;
}

/*
 * On state change, update the button structure and callback to client
 */
static void onStateChange(BTN * btn, BTN_STATE_ENUM new_state)
{
	btn->current_state = new_state;
	btn->change_state_callback(btn->current_state);
	btn->repeat_count = 0;
}

/*
 * No change to state, only callback if active
 */
static void onSameState(BTN * btn)
{
	if (btn->current_state == BTN_STATE_ACTIVE)
	{
		// Count how long the active state has persisted
		
		if (++btn->repeat_count == btn->max_repeat_count)
		{
			btn->repeat_callback();
			btn->repeat_count = 0;
		}
	}
}

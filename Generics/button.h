#ifndef _BUTTON_H_
#define _BUTTON_H_

/*
 * Public Datatype Definitions
 */

enum btn_state_enum
{
	BTN_STATE_ACTIVE,
	BTN_STATE_INACTIVE,
};
typedef enum btn_state_enum BTN_STATE_ENUM;

typedef void (*button_rpt_fn)(void);
typedef void (*button_chg_fn)(BTN_STATE_ENUM state);

struct btn_struct
{
	BTN_STATE_ENUM			current_state;
	button_chg_fn			change_state_callback;
	button_rpt_fn			repeat_callback;
	uint16_t				repeat_count;
	const uint16_t			max_repeat_count;
	uint16_t				debounce_count;
	const uint16_t			max_debounce_count;
};
typedef struct btn_struct BTN;

bool BTN_InitHandler(BTN * btn);
void BTN_Update(BTN * btn, BTN_STATE_ENUM state);

#endif

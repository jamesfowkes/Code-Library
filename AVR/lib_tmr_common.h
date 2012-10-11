#ifndef _LIB_TMR_H_
#define _LIB_TMR_H_

/*
 * Public Datatypes
 */
enum tmr_src_enum
{
	TMR_SRC_OFF,
	TMR_SRC_FCLK,
	TMR_SRC_FCLK_8,
	TMR_SRC_FCLK_64,
	TMR_SRC_FCLK_256,
	TMR_SRC_FCLK_1024,
	TMR_SRC_FCLK_EXT_FALLING,
	TMR_SRC_FCLK_EXT_RISING,
};
typedef enum tmr_src_enum TMR_SRC_ENUM;

/*
 * Public Function Prototypes
 */
void TMR_SyncModeControl(bool sync);

#endif

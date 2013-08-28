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

enum tmr_occhan_enum
{
	TMR_OCCHAN_A,
	TMR_OCCHAN_B,
	TMR_OCCHAN_INVALID,
};
typedef enum tmr_occhan_enum TMR_OCCHAN_ENUM;

enum tmr_outputmode_enum
{
	TMR_OUTPUTMODE_NONE,
	TMR_OUTPUTMODE_TOGGLE,
	TMR_OUTPUTMODE_CLEAR,
	TMR_OUTPUTMODE_SET,
	TMR_OUTPUTMODE_INVALID
};
typedef enum tmr_outputmode_enum TMR_OUTPUTMODE_ENUM;

/*
 * Public Function Prototypes
 */

void TMR_SyncModeControl(bool sync);
void TMR_SetOutputPorts(TMR_OCCHAN_ENUM eChannel, IO_PORT_ENUM ePort, uint8_t pin);
void TMR_PWMOff(const TMR_OCCHAN_ENUM eChannel, const IO_STATE_ENUM eState);

#endif

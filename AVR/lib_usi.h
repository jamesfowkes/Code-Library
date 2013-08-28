#ifndef _LIB_USI_H_
#define _LIB_USI_H_

enum usi_mode_enum
{
	USI_MODE_NONE,
	USI_MODE_THREE_WIRE,
	USI_MODE_TWO_WIRE_1,
	USI_MODE_TWO_WIRE_2
};
typedef enum usi_mode_enum USI_MODE_ENUM;

enum usi_clk_src_enum
{
	USI_CLK_SRC_NONE,
	USI_CLK_SRC_USICLK,
	USI_CLK_SRC_TMR0,
	USI_CLK_SRC_EXT_POS,
	USI_CLK_SRC_EXT_NEG
};
typedef enum usi_clk_src_enum USI_CLK_SRC_ENUM;

enum usi_count_src_enum
{
	USI_COUNT_SRC_NONE,
	USI_COUNT_SRC_USICLK,
	USI_COUNT_SRC_TMR0,
	USI_COUNT_SRC_EXT,
	USI_COUNT_SRC_USITC
};
typedef enum usi_count_src_enum USI_COUNT_SRC_ENUM;

enum usi_status_flag_enum
{
	USI_STATUS_START_CONDITION,
	USI_STATUS_OVERFLOW,
	USI_STATUS_STOP_CONDITION,
	USI_STATUS_DATA_COLLISION,
};
typedef enum usi_status_flag_enum USI_STATUS_FLAG_ENUM;

/* This MUST match definition of SPI_DATA ! */
struct usi_data_struct
{
	bool transferComplete;
	uint8_t byte;
};
typedef struct usi_data_struct USI_DATA;

void USI_SetMode(USI_MODE_ENUM mode);
void USI_StartInterruptEnable(bool enable);
void USI_CounterInterruptEnable(bool enable);
void USI_SetSources(USI_CLK_SRC_ENUM clockSource, USI_COUNT_SRC_ENUM countSource);

bool USI_GetStatusFlag(USI_STATUS_FLAG_ENUM flag);
uint8_t USI_GetCounterValue(void);

uint8_t USI_SendByte(uint8_t byte);
void USI_SetReply(uint8_t byte, USI_DATA * data);

#endif


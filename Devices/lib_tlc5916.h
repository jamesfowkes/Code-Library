#ifndef _LIB_TLC5916_H_
#define _LIB_TLC5916_H_

struct tlc5916_control_struct
{
	SR_CONTROL sr;
	SR_IO_FN latch;
	SR_IO_FN oe;
};
typedef struct tlc5916_control_struct TLC5916_CONTROL;

enum tlc5916_mode_enum
{
	MODE_NORMAL,
	MODE_SPECIAL
};
typedef enum tlc5916_mode_enum TLC5916_MODE_ENUM;

void TLC5916_Init(TLC5916_CONTROL * tlc, SR_SHIFT_FN shiftOutFn, SR_IO_FN tlcLatchFn, SR_IO_FN tlcOEFn);
void TLC5916_ClockOut(uint8_t* data, uint8_t nBytes, TLC5916_CONTROL * tlc);
void TLC5916_OutputEnable(TLC5916_CONTROL * tlc, bool enable);

void TLC5916_SetMode(TLC5916_CONTROL * tlc, TLC5916_MODE_ENUM eMode);
uint8_t TLC5916_MakeCurrentGainByte(uint8_t percent_gain, bool high_current);

#endif

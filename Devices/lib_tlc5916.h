#ifndef _LIB_TLC5916_H_
#define _LIB_TLC5916_H_

struct tlc5916_control_struct
{
	SR_CONTROL sr;
	EXT_IO_FN latch;
	EXT_IO_FN oe;
};
typedef struct tlc5916_control_struct TLC5916_CONTROL;

void TLC5916_ClockOut(uint8_t* data, uint8_t nBytes, TLC5916_CONTROL * tlc);
void TLC5916_OutputEnable(TLC5916_CONTROL * tlc, bool enable);

#endif

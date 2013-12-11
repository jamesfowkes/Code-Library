#ifndef _LIB_SHIFTREGISTER_H_
#define _LIB_SHIFTREGISTER_H_

void SR_Init(uint8_t * clockPort, uint8_t clockPin, uint8_t * dataPort, uint8_t dataPin);
void SR_ShiftOut(uint8_t* data, uint8_t nBytes, SR_CLKEDGE_ENUM edge, SR_DIRECTION_ENUM dir);

#endif

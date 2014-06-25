#ifndef _FORMAT_H_
#define _FORMAT_H_

uint8_t TranslateBuffer(char const * const buffer, char * outBuffer, uint8_t size, void * args[], uint8_t nargs, bool nullTerminate);

uint8_t S8ToString(int8_t val, char * buf);
uint8_t U8ToString(uint8_t val, char * buf);
uint8_t S16ToString(int16_t val, char * buf);
uint8_t U16ToString(uint16_t val, char * buf);

#ifdef FORMAT_32_BIT_INTS
uint8_t S32ToString(int32_t val, char * buf);
uint8_t U32ToString(uint32_t val, char * buf);
#endif

void RightAlign(char * buf);

#endif
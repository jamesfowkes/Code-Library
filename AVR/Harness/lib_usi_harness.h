#ifndef _LIB_USI_HARNESS_H_
#define _LIB_USI_HARNESS_H_

void USI_Harness_TriggerISR(USI_DATA * usi, uint8_t data);
void USI_Harness_KickOverflowInMs(uint32_t ms);

#endif

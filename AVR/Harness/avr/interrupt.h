#ifndef _INTERRUPT_H_
#define _INTERRUPT_H_

#include "avr/io_harness_macros.h"

#define cli() {}
#define sei() {}

#define ISR(name) void name ## isr(void)

#endif

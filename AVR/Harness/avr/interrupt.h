#ifndef _INTERRUPT_H_
#define _INTERRUPT_H_

uint8_t dummy;

#define cli() {}
#define sei() {}

#define GIMSK	(dummy)

#define ISR(name) void name(void)

#endif

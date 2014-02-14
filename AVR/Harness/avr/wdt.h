#ifndef _WDT_H_
#define _WDT_H_

uint8_t dummy;

#define MCUSR	(dummy)
#define WDRF	(0)

#define wdt_disable() {}

#endif

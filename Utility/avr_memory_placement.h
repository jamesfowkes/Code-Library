#ifndef _AVR_MEMORY_PLACEMENT_H_
#define _AVR_MEMORY_PLACEMENT_H_

/* This should not be directly included by applications!
Use the generic util_memory_placement.h instead! */

#include <avr/pgmspace.h>

#define IN_PMEM(x) x PROGMEM
#define	PMEM_STRNCPY strncpy_P
#define STRNCMP strncmp_P

#define IN_EEPROM(x) EEMEM x

#endif

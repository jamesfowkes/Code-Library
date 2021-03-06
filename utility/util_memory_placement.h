#ifndef _UTIL_MEMORY_PLACEMENT_H_
#define _UTIL_MEMORY_PLACEMENT_H_

#if defined(__AVR__)
#include "avr_memory_placement.h"

#elif defined(TEST_HARNESS)

#include "harness_memory_placement.h"

#else

#define IN_PMEM(x) x
#define	PMEM_STRNCPY strncpy
#define STRNCMP strncmp

#define IN_EEPROM(x) x

#endif

#endif /* _UTIL_MEMORY_PLACEMENT_H_ */

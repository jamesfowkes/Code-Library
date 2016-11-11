#ifndef _HARNESS_MEMORY_PLACEMENT_H_
#define _HARNESS_MEMORY_PLACEMENT_H_

/* This should not be directly included by applications!
Use the generic util_memory_placement.h instead! */

#define pgm_read_byte(x) (*x)
#define IN_PMEM(x) x
#define	PMEM_STRNCPY strncpy
#define STRNCMP strncmp

#define IN_EEPROM(x) x

#endif

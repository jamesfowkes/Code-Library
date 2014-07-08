/*
 * Standard Library Includes
 */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <assert.h>
#include <time.h>
/*
 * AVR Includes (Defines and Primitives)
 */
 
#include <avr/io.h>
#include <avr/eeprom.h>

#include "lib_eeprom_harness.h"

uint8_t 	eeprom_read_byte (const uint8_t *addr) { return *addr; }

uint16_t 	eeprom_read_word (const uint16_t *addr) { return *addr; }

void 	eeprom_read_block (void *pointer_ram, const void *pointer_eeprom, size_t n)
{
	memcpy(pointer_ram, pointer_eeprom, n);
}

void 	eeprom_write_byte (uint8_t *addr, uint8_t value) { *addr = value; }
void 	eeprom_write_word (uint16_t *addr, uint16_t value)  { *addr = value; }
void 	eeprom_write_block (const void *pointer_ram, void *pointer_eeprom, size_t n)
{
	memcpy(pointer_eeprom, pointer_ram, n);
}

/*
 * lib_fuses.c
 *
 *  Created on: 4 Mar 2013
 *      Author: james
 *
 * Copied with minor alterations from
 * http://embeddedgurus.com/stack-overflow/2009/05/checking-the-fuse-bits-in-an-atmel-avr-at-run-time/
 */

#include <stdint.h>

#include "lib_fuses.h"

/* AVR-GCC/avr-libc */
#  include <avr/boot.h>
#  include <avr/pgmspace.h>

#if defined(GET_LOCK_BITS)    /* avr-libc >= 1.2.5 */
	#define _GET_LOCK_BITS() boot_lock_fuse_bits_get(GET_LOCK_BITS)
	#define _GET_LOW_FUSES() boot_lock_fuse_bits_get(GET_LOW_FUSE_BITS)
	#define _GET_HIGH_FUSES() boot_lock_fuse_bits_get(GET_HIGH_FUSE_BITS)
	#define _GET_EXTENDED_FUSES() boot_lock_fuse_bits_get(GET_EXTENDED_FUSE_BITS)
#endif /* defined(GET_LOCK_BITS) */
#define _SET_LOCK_BITS(data) boot_lock_bits_set(~data)
#define _ENABLE_RWW_SECTION() boot_rww_enable()

#define _WAIT_FOR_SPM() boot_spm_busy_wait()

#ifndef LARGE_MEMORY
	#define _LOAD_PROGRAM_MEMORY(addr) pgm_read_byte_near(addr)
#else /* LARGE_MEMORY */
	#define _LOAD_PROGRAM_MEMORY(addr) pgm_read_byte_far(addr)
#endif /* LARGE_MEMORY */
#define _FILL_TEMP_WORD(addr,data) boot_page_fill(addr, data)
#define _PAGE_ERASE(addr) boot_page_erase(addr)
#define _PAGE_WRITE(addr) boot_page_write(addr)

static FUSE_SETTINGS fuses;

FUSE_SETTINGS * FUS_GetFuses(void)
{
	fuses.fuse_low = _GET_LOW_FUSES();
	fuses.fuse_high = _GET_HIGH_FUSES();
	fuses.fuse_extended = _GET_EXTENDED_FUSES();
	fuses.lockbits = _GET_LOCK_BITS();

	return &fuses;
}

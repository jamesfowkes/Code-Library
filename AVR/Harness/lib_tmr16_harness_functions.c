/*
 * Standard Library Includes
 */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>

#include <pthread.h>

/*
 * AVR Includes
 */

#include <avr/io.h>

/*
 * AVR Library Includes
 */

#include "lib_io.h"
#include "lib_tmr16.h"
#include "lib_tmr16_harness_functions.h"

/*
 * Private Variables
 */

uint8_t TCCR1A;
uint8_t TCCR1B;
uint8_t TCCR1C;
uint16_t TCNT1;
uint16_t ICR1;
uint8_t ICR1L;
uint8_t ICR1H;
uint16_t OCR1A;
uint8_t OCR1AL;
uint8_t OCR1AH;
uint16_t OCR1B;
uint8_t OCR1BL;
uint8_t OCR1BH;
uint8_t TIMSK1;
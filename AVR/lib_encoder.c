/*
 * Standard Library Includes
 */

#include <stdbool.h>
#include <stdint.h>

/*
 * AVR Includes (Defines and Primitives)
 */
#include <avr/io.h>
#include <avr/interrupt.h>

/*
 * AVR Library Includes
 */

#include "lib_io.h" 
#include "lib_encoder.h" 
#include "lib_pcint.h" 

#if !defined(ENCODER_PCINT0)
#if !defined(ENCODER_PCINT1)
#if !defined(ENCODER_PCINT2)
#if !defined(ENCODER_PCINT3)
#error "At least one of ENCODER_PCINT0, ENCODER_PCINT1, ENCODER_PCINT2, ENCODER_PCINT3 must be defined!"
#endif
#endif
#endif
#endif

/*
 * Local Variables
 */
 
static volatile uint8_t * s_port;
static uint8_t s_masks[3];
static uint8_t s_testValue;

static int s_lastReadCount = 0;
static volatile int s_count = 0;
static volatile int s_debouncedCount = 0;

static uint8_t s_debounce = 1;

/*
 * Private Function Prototypes
 */

 static inline void updateEncoder(void) __attribute__((always_inline));

/*
 * Public Function Definitions
 */

/* ENC_Setup
 :Sets up encoder on a single port, pins A and B
*/
void ENC_Setup(IO_PORT_ENUM ePort, uint8_t A, uint8_t B, uint8_t interruptA, uint8_t interruptB, uint8_t debounceCount)
{

	s_debounce = debounceCount;
	IO_SetMode(ePort, A, IO_MODE_PULLUPINPUT);
	IO_SetMode(ePort, B, IO_MODE_PULLUPINPUT);

	s_port = IO_GetReadPortDirect(ePort); // Get a direct pointer to the port register
	// Create masks for the encoder channel bits
	s_masks[0] = (1 << A);
	s_masks[1] = (1 << B);
	s_masks[2] = s_masks[0] | s_masks[1];

	// Get the starting encoder value (translate to ABCD format described in updateEncoder)
	uint8_t newValue = (*s_port) & s_masks[2];
	s_testValue |= (newValue & s_masks[0]) ? 2 : 0;
	s_testValue |= (newValue & s_masks[1]) ? 1 : 0;

	PCINT_EnableInterrupt(interruptA, true);
	PCINT_EnableInterrupt(interruptB, true);
}

/* ENC_GetMovement
 :Returns difference between last time the encoder was read and the latest value
 */
int ENC_GetMovement(void)
{
	int change = s_debouncedCount - s_lastReadCount;
	s_lastReadCount = s_debouncedCount;
	return change;
}

#ifdef TEST_HARNESS
/* ENC_SetMovement
 :For test harness only, sets the movement of the encoder so that applications may be tested
 */
void ENC_SetMovement(int movement)
{
	s_debouncedCount += movement;
}

#endif

/*
 * Private Function Definitions
 */

/* updateEncoder
 :Should be inlined in ISR.
 :Updates the encoder count
 */
static inline void updateEncoder(void)
{
	uint8_t newValue = (*s_port) & s_masks[2];
	
	s_testValue <<= 2;
	s_testValue |= (newValue & s_masks[0]) ? 2 : 0;
	s_testValue |= (newValue & s_masks[1]) ? 1 : 0;
	
	/* s_testValue now contains a pattern ABCD in lowest four bits where:
	A = previous A channel value
	B = previous B channel value
	C = new A channel value
	D = new B channel value
	*/
	
	switch (s_testValue & 0x0F)
	{
	case 1:
	case 7:
	case 8:
	case 14:
		s_count++; // Forwards
		if (s_count >= s_debounce) {s_debouncedCount++; s_count = 0;}
		break;
	case 2:
	case 4:
	case 11:
	case 13:
		s_count--; // Backwards
		if (s_count <= -s_debounce) {s_debouncedCount--; s_count = 0;}
		break;
	default:
		break;
	}
}

/*
 * ISR Functions
 */
 
#if defined(ENCODER_PCINT0)
ISR(PCINT0_vect)
{
	updateEncoder();
}
#endif

#if defined(ENCODER_PCINT1)
ISR(PCINT1_vect)
{
	updateEncoder();
}
#endif

#if defined(ENCODER_PCINT2)
ISR(PCINT2_vect)
{
	updateEncoder();
}
#endif

#if defined(ENCODER_PCINT3)
ISR(PCINT3_vect)
{
	updateEncoder();
}
#endif

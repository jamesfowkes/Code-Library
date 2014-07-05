#ifndef _LIB_ENCODER_H_
#define _LIB_ENCODER_H_

/*
 * Public Function Prototypes
 */
void ENC_Setup(IO_PORT_ENUM ePort, uint8_t A, uint8_t B, uint8_t interruptA, uint8_t interruptB, uint8_t debounceCount);
int ENC_GetMovement(void);

#ifdef TEST_HARNESS
void ENC_SetMovement(int movement);
#endif

#endif

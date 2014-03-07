#ifndef _UTIL_SEQUENCE_GENERATOR_H_
#define _UTIL_SEQUENCE_GENERATOR_H_

typedef struct sequence SEQUENCE;

SEQUENCE * SEQGEN_GetNewSequence(uint16_t length);
void SEQGEN_AddConstants(SEQUENCE * seq, uint16_t value, uint16_t length);
uint16_t SEQGEN_Read(SEQUENCE * seq);

void SEQGEN_AddRamp_StartStepLength(SEQUENCE * seq, uint16_t strt, uint16_t step, uint16_t length);

#ifdef TEST_HARNESS
uint16_t SEQGENspy_GetFromIndex(SEQUENCE * seq, uint16_t index);
#endif

#endif

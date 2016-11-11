#ifndef _UTIL_SEQUENCE_GENERATOR_H_
#define _UTIL_SEQUENCE_GENERATOR_H_

typedef struct sequence SEQUENCE;

SEQUENCE * SEQGEN_GetNewSequence(uint16_t length);
void SEQGEN_AddConstants(SEQUENCE * seq, int16_t value, uint16_t length);

int16_t SEQGEN_Read(SEQUENCE * seq);

void SEQGEN_AddRamp_StartStepLength(SEQUENCE * seq, int16_t strt, int16_t step, uint16_t length);

void SEQGEN_AddNoise(SEQUENCE * seq, int16_t amplitude);

bool SEQGEN_EOS(SEQUENCE * seq);

#ifdef TEST_HARNESS
int16_t SEQGENspy_GetFromIndex(SEQUENCE * seq, uint16_t index);
int16_t SEQGENspy_GetWriteIndex(SEQUENCE * seq);
int16_t SEQGENspy_GetReadIndex(SEQUENCE * seq);
int16_t SEQGENspy_GetMaxReadIndex(SEQUENCE * seq);
#endif

#endif

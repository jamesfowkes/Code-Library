/*
 * Standard Library Includes
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>

/*
 * Generic Library Includes
 */

#include "memorypool.h"

/*
 * Utility Library Includes
 */

#include "util_macros.h"
#include "util_sequence_generator.h"

/*
 * Defines and Typedefs
 */
 
struct sequence
{
	void * data;
	int16_t maxReadIndex;
	uint16_t maxLength;
	uint16_t wIndex;
	uint16_t rIndex;
	bool eos;
};

/*
 * Public Function Defintions
 */

static void writeValue(SEQUENCE * seq, int16_t value)
{
	((uint16_t*)seq->data)[seq->wIndex] = value;
	incrementwithrollover(seq->wIndex, seq->maxLength-1);
	seq->maxReadIndex++;
	seq->maxReadIndex = min(seq->maxReadIndex, seq->maxLength);
}

SEQUENCE * SEQGEN_GetNewSequence(uint16_t length)
{
	SEQUENCE * seq = (SEQUENCE *)MEMPOOL_GetBytes(sizeof(SEQUENCE));
	if (seq)
	{
		seq->data = (void*)MEMPOOL_GetBytes(length * sizeof(uint16_t));
		if (seq->data)
		{
			seq->maxLength = length;
			seq->maxReadIndex = -1;
			seq->wIndex = 0;
			seq->rIndex = 0;
			seq->eos = false;
		}
		else
		{
			seq = NULL;
		}
	}
	
	return seq;
}

void SEQGEN_AddConstants(SEQUENCE * seq, int16_t value, uint16_t length)
{
	if (seq)
	{
		for (uint16_t i = 0; i < length; ++i)
		{
			writeValue(seq, value);
		}
	}
}

void SEQGEN_AddNoise(SEQUENCE * seq, int16_t amplitude)
{
	int noise;
	
	if (seq)
	{
		if (seq->maxReadIndex >= 0)
		{
			for (uint16_t i = 0; i < seq->maxReadIndex; ++i)
			{
				noise = (rand() % amplitude) - (amplitude / 2);
				((uint16_t*)seq->data)[i] += noise;
			}
		}
	}	
}

int16_t SEQGEN_Read(SEQUENCE * seq)
{
	uint16_t value = 0;
	
	if (seq)
	{
		if (seq->maxReadIndex >= 0)
		{
			value = ((int16_t*)seq->data)[seq->rIndex];
			incrementwithrollover(seq->rIndex, seq->maxReadIndex);
			seq->eos = (seq->rIndex == 0);
		}
	}
	
	return value;
}

bool SEQGEN_EOS(SEQUENCE * seq)
{
	return seq->eos;
}

void SEQGEN_AddRamp_StartStepLength(SEQUENCE * seq, int16_t strt, int16_t step, uint16_t length)
{
	uint16_t value = strt;
	
	if (seq)
	{
		for (uint16_t i = 0; i < length; ++i)
		{
			writeValue(seq, value);
			value += step;
		}
	}
}

#ifdef TEST_HARNESS
int16_t SEQGENspy_GetFromIndex(SEQUENCE * seq, uint16_t index)
{
	return ((int16_t*)seq->data)[index];
}

int16_t SEQGENspy_GetWriteIndex(SEQUENCE * seq) { return seq->wIndex; }
int16_t SEQGENspy_GetReadIndex(SEQUENCE * seq) { return seq->rIndex; }
int16_t SEQGENspy_GetMaxReadIndex(SEQUENCE * seq) { return seq->maxReadIndex; }

#endif



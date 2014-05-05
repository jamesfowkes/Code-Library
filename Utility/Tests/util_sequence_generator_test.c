#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

#include "unity.h"

#include "memorypool.h"
#include "../util_sequence_generator.h"

SEQUENCE * sequence = NULL;

void setUp(void)
{
	MEMPOOL_Reset();
	sequence = SEQGEN_GetNewSequence(1024);
}

void tearDown(void)
{

}

void test_GenerateEmptySequence(void)
{
	TEST_ASSERT_NOT_NULL(sequence);
	SEQGEN_AddConstants(sequence, 0, 1024);
	
	for (uint16_t i = 0; i < 1024; ++i)
	{
		TEST_ASSERT_EQUAL(0, SEQGEN_Read(sequence));
	}
}

void test_GenerateConstantSequence(void)
{
	SEQGEN_AddConstants(sequence, 50, 256);
	TEST_ASSERT_EQUAL(256, SEQGENspy_GetWriteIndex(sequence));
	TEST_ASSERT_EQUAL(255, SEQGENspy_GetMaxReadIndex(sequence));
	
	SEQGEN_AddConstants(sequence, 50, 256);
	TEST_ASSERT_EQUAL(512, SEQGENspy_GetWriteIndex(sequence));
	TEST_ASSERT_EQUAL(511, SEQGENspy_GetMaxReadIndex(sequence));
	
	for (uint16_t i = 0; i < 512; ++i)
	{
		TEST_ASSERT_EQUAL(i, SEQGENspy_GetReadIndex(sequence));
		TEST_ASSERT_EQUAL(50, SEQGEN_Read(sequence));
		TEST_ASSERT_EQUAL(i == 511 ? 0 : i+1, SEQGENspy_GetReadIndex(sequence));
	}
	
	TEST_ASSERT_EQUAL(true, SEQGEN_EOS(sequence));
	TEST_ASSERT_EQUAL(50, SEQGEN_Read(sequence));
}

void test_GenerateConstantSequenceWithRollover(void)
{
	SEQGEN_AddConstants(sequence, 100, 1023);
	SEQGEN_AddConstants(sequence, 200, 2);
	
	TEST_ASSERT_EQUAL(100, SEQGENspy_GetFromIndex(sequence, 1022));
	TEST_ASSERT_EQUAL(200, SEQGENspy_GetFromIndex(sequence, 1023));
	TEST_ASSERT_EQUAL(200, SEQGENspy_GetFromIndex(sequence, 0));
	TEST_ASSERT_EQUAL(100, SEQGENspy_GetFromIndex(sequence, 1));
}

void test_GenerateRisingLinearRampWithUnityStep(void)
{
	SEQGEN_AddRamp_StartStepLength(sequence, 1, 1, 100);
	
	TEST_ASSERT_EQUAL(1, SEQGENspy_GetFromIndex(sequence, 0));
	TEST_ASSERT_EQUAL(100, SEQGENspy_GetFromIndex(sequence, 99));
	TEST_ASSERT_EQUAL(0, SEQGENspy_GetFromIndex(sequence, 100));
}

void test_GenerateRisingLinearRampWithBigStep(void)
{
	SEQGEN_AddRamp_StartStepLength(sequence, 1, 10, 100);
	
	TEST_ASSERT_EQUAL(1, SEQGENspy_GetFromIndex(sequence, 0));
	TEST_ASSERT_EQUAL(991, SEQGENspy_GetFromIndex(sequence, 99));
	TEST_ASSERT_EQUAL(0, SEQGENspy_GetFromIndex(sequence, 100));
}

void test_GenerateFallingLinearRampWithUnityStep(void)
{
	SEQGEN_AddRamp_StartStepLength(sequence, 100, -1, 100);
	
	TEST_ASSERT_EQUAL(100, SEQGENspy_GetFromIndex(sequence, 0));
	TEST_ASSERT_EQUAL(1, SEQGENspy_GetFromIndex(sequence, 99));
	TEST_ASSERT_EQUAL(0, SEQGENspy_GetFromIndex(sequence, 100));
}

void test_GenerateFallingLinearRampWithBigStep(void)
{
	SEQGEN_AddRamp_StartStepLength(sequence, 991, -10, 100);
	
	TEST_ASSERT_EQUAL(991, SEQGENspy_GetFromIndex(sequence, 0));
	TEST_ASSERT_EQUAL(1, SEQGENspy_GetFromIndex(sequence, 99));
	TEST_ASSERT_EQUAL(0, SEQGENspy_GetFromIndex(sequence, 100));
}

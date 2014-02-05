#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

#include "unity.h"

#define SM_PRIVATE_ACCESS
#include "../../ringbuf.h"
#include "../../statemachine.h"
#include "../../statemachinemanager.h"

static int8_t index = 0UL;

enum states
{
	S1,
	S2,
	S3,
	S4,
	SMAX
};

enum events
{
	E1,
	E2,
	E3,
	E4,
	EMAX
};

static void onS1Enter(SM_STATEID, SM_STATEID, SM_EVENT);
static void onS1Leave(SM_STATEID, SM_STATEID, SM_EVENT);

static void onS2Leave(SM_STATEID, SM_STATEID, SM_EVENT);

static void onS3Enter(SM_STATEID, SM_STATEID, SM_EVENT);

static void onE1S1(SM_STATEID, SM_STATEID, SM_EVENT);
static void onE2S1(SM_STATEID, SM_STATEID, SM_EVENT);
static void onE3S1(SM_STATEID, SM_STATEID, SM_EVENT);

static void onE4S2(SM_STATEID, SM_STATEID, SM_EVENT);
static void onE4S3(SM_STATEID, SM_STATEID, SM_EVENT);
static void onE4S4(SM_STATEID, SM_STATEID, SM_EVENT);

static SM_STATE states[] = {
	{S1, onS1Leave, onS1Enter	},
	{S2, onS2Leave,	NULL		},
	{S3, NULL, 		onS3Enter	},
	{S4, NULL,		NULL		},
};

static const SM_ENTRY sm[] = {
	{&states[S1], E1, onE1S1, &states[S2]},	
	{&states[S1], E2, onE2S1, &states[S3]},
	{&states[S1], E3, onE3S1, &states[S4]},
	
	{&states[S2], E4, onE4S2, &states[S1]},
	{&states[S3], E4, onE4S3, &states[S1]},
	{&states[S4], E4, onE4S4, &states[S1]}
};

static SM_FUNCTION fnCallHistory[100];
static uint32_t h = 0UL;

static void clearHistory(void)
{
	int i = 0;
	for (; i < 100; ++i)
	{
		fnCallHistory[i] = NULL;
	}
	h = 0;
}

void setUp(void)
{
	clearHistory();
}

void tearDown(void)
{

}

void test_GetStateMachinePointers(void)
{
	printf("Size of state machine = %d, size of event = %d\n", sizeof(STATE_MACHINE_INTERNAL), sizeof(SM_EVENT));
	TEST_ASSERT( SMM_Config(2, 20) );
}

void test_StateMachineInit(void)
{
	index = SM_Init(&states[S1], EMAX, SMAX, &sm[0]);
	TEST_ASSERT(index > -1);
}

void test_StateMachineRunning(void)
{
	index = SM_Init(&states[S1], EMAX, SMAX, &sm[0]);
	
	SM_SetActive(index, true);
	
	SM_Event(index, E1);
	TEST_ASSERT_EQUAL_PTR(onS1Leave, fnCallHistory[0]);
	TEST_ASSERT_EQUAL_PTR(onE1S1, fnCallHistory[1]);
	TEST_ASSERT_EQUAL(S2, SM_GetState(index));
	
	SM_Event(index, E4);
	TEST_ASSERT_EQUAL_PTR(onS2Leave, fnCallHistory[2]);
	TEST_ASSERT_EQUAL_PTR(onE4S2, fnCallHistory[3]);
	TEST_ASSERT_EQUAL_PTR(onS1Enter, fnCallHistory[4]);
	TEST_ASSERT_EQUAL(S1, SM_GetState(index));
	
	SM_Event(index, E2);
	TEST_ASSERT_EQUAL_PTR(onS1Leave, fnCallHistory[5]);
	TEST_ASSERT_EQUAL_PTR(onE2S1, fnCallHistory[6]);
	TEST_ASSERT_EQUAL_PTR(onS3Enter, fnCallHistory[7]);
	TEST_ASSERT_EQUAL(S3, SM_GetState(index));
	
	SM_Event(index, E4);
	TEST_ASSERT_EQUAL_PTR(onE4S3, fnCallHistory[8]);
	TEST_ASSERT_EQUAL_PTR(onS1Enter, fnCallHistory[9]);
	TEST_ASSERT_EQUAL(S1, SM_GetState(index));
	
	SM_Event(index, E3);
	TEST_ASSERT_EQUAL_PTR(onS1Leave, fnCallHistory[10]);
	TEST_ASSERT_EQUAL_PTR(onE3S1, fnCallHistory[11]);
	TEST_ASSERT_EQUAL(S4, SM_GetState(index));
	
	SM_Event(index, E4);
	TEST_ASSERT_EQUAL_PTR(onE4S4, fnCallHistory[12]);
	TEST_ASSERT_EQUAL_PTR(onS1Enter, fnCallHistory[13]);
	TEST_ASSERT_EQUAL(S1, SM_GetState(index));
}

static void storeHistory(const char * name, SM_FUNCTION fn)
{
	fnCallHistory[h++] = fn;
}

static void onS1Enter(SM_STATEID old, SM_STATEID new, SM_EVENT e){ storeHistory(__func__, onS1Enter); }

static void onS1Leave(SM_STATEID old, SM_STATEID new, SM_EVENT e){ storeHistory(__func__, onS1Leave); }

static void onS2Leave(SM_STATEID old, SM_STATEID new, SM_EVENT e){ storeHistory(__func__, onS2Leave); }

static void onS3Enter(SM_STATEID old, SM_STATEID new, SM_EVENT e){ storeHistory(__func__, onS3Enter); }

static void onE1S1(SM_STATEID old, SM_STATEID new, SM_EVENT e){ storeHistory(__func__, onE1S1); }
static void onE2S1(SM_STATEID old, SM_STATEID new, SM_EVENT e){ storeHistory(__func__, onE2S1); }
static void onE3S1(SM_STATEID old, SM_STATEID new, SM_EVENT e){ storeHistory(__func__, onE3S1); }

static void onE4S2(SM_STATEID old, SM_STATEID new, SM_EVENT e){ storeHistory(__func__, onE4S2); }
static void onE4S3(SM_STATEID old, SM_STATEID new, SM_EVENT e){ storeHistory(__func__, onE4S3); }
static void onE4S4(SM_STATEID old, SM_STATEID new, SM_EVENT e){ storeHistory(__func__, onE4S4); }

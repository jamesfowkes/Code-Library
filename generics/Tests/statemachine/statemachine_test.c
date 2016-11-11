#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

#include "unity.h"

#define SM_PRIVATE_ACCESS
#include "../../ringbuf.h"
#include "../../statemachine.h"
#include "../../statemachinemanager.h"

#define VOIDALL (void)old; (void)new, (void)e

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

static void onE5S4(SM_STATEID, SM_STATEID, SM_EVENT);
static void onE6S5(SM_STATEID, SM_STATEID, SM_EVENT);
static void onE7S6(SM_STATEID, SM_STATEID, SM_EVENT);

DEFINE_SM_STATES(S1, S2, S3, S4, S5, S6);
DEFINE_SM_EVENTS(E1, E2, E3, E4, E5, E6, E7);

DEFINE_STATES(sm) = {
	{S1, onS1Leave, onS1Enter	},
	{S2, onS2Leave,	NULL		},
	{S3, NULL, 	onS3Enter	},
	{S4, NULL,	NULL		},
	{S5, NULL,	NULL		},
	{S6, NULL,	NULL		},
};

DEFINE_STATE_TRANSITIONS(sm) = {
	STATE_TRANSITION(sm, S1, E1, onE1S1, S2),
	STATE_TRANSITION(sm, S1, E2, onE2S1, S3),
	STATE_TRANSITION(sm, S1, E3, onE3S1, S4),

	STATE_TRANSITION(sm, S2, E4, onE4S2, S1),
	STATE_TRANSITION(sm, S3, E4, onE4S3, S1),
	STATE_TRANSITION(sm, S4, E4, onE4S4, S1),

	STATE_TRANSITION(sm, S4, E5, onE5S4, S5),
	STATE_TRANSITION(sm, S5, E6, onE6S5, S6),
	STATE_TRANSITION(sm, S6, E7, onE7S6, S1)
};

DEFINE_STATE_MACHINE(sm, S1);

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
	TEST_ASSERT( SMM_Config(3, 10) );
}

void test_StateMachineInit(void)
{
	SM_Init(&sm);
	TEST_ASSERT(SM_GetState(&sm) > -1);
}

void test_StateMachineRunning(void)
{
	SM_Init(&sm);
	
	SM_SetActive(&sm, true);
	
	SM_Event(&sm, E1);
	TEST_ASSERT_EQUAL_PTR(onS1Leave, fnCallHistory[0]);
	TEST_ASSERT_EQUAL_PTR(onE1S1, fnCallHistory[1]);
	TEST_ASSERT_EQUAL(S2, SM_GetState(&sm));
	
	SM_Event(&sm, E4);
	TEST_ASSERT_EQUAL_PTR(onS2Leave, fnCallHistory[2]);
	TEST_ASSERT_EQUAL_PTR(onE4S2, fnCallHistory[3]);
	TEST_ASSERT_EQUAL_PTR(onS1Enter, fnCallHistory[4]);
	TEST_ASSERT_EQUAL(S1, SM_GetState(&sm));
	
	SM_Event(&sm, E2);
	TEST_ASSERT_EQUAL_PTR(onS1Leave, fnCallHistory[5]);
	TEST_ASSERT_EQUAL_PTR(onE2S1, fnCallHistory[6]);
	TEST_ASSERT_EQUAL_PTR(onS3Enter, fnCallHistory[7]);
	TEST_ASSERT_EQUAL(S3, SM_GetState(&sm));
	
	SM_Event(&sm, E4);
	TEST_ASSERT_EQUAL_PTR(onE4S3, fnCallHistory[8]);
	TEST_ASSERT_EQUAL_PTR(onS1Enter, fnCallHistory[9]);
	TEST_ASSERT_EQUAL(S1, SM_GetState(&sm));
	
	SM_Event(&sm, E3);
	TEST_ASSERT_EQUAL_PTR(onS1Leave, fnCallHistory[10]);
	TEST_ASSERT_EQUAL_PTR(onE3S1, fnCallHistory[11]);
	TEST_ASSERT_EQUAL(S4, SM_GetState(&sm));
	
	SM_Event(&sm, E4);
	TEST_ASSERT_EQUAL_PTR(onE4S4, fnCallHistory[12]);
	TEST_ASSERT_EQUAL_PTR(onS1Enter, fnCallHistory[13]);
	TEST_ASSERT_EQUAL(S1, SM_GetState(&sm));
}

void test_StateMachineReentrant(void)
{
	SM_Init(&sm);
	
	SM_SetActive(&sm, true);

	SM_Event(&sm, E3);
	TEST_ASSERT_EQUAL_PTR(onS1Leave, fnCallHistory[0]);
	TEST_ASSERT_EQUAL_PTR(onE3S1, fnCallHistory[1]);

	SM_Event(&sm, E5);
}

static void storeHistory(const char * name, SM_FUNCTION fn)
{
	(void)name;
	fnCallHistory[h++] = fn;
}

static void onS1Enter(SM_STATEID old, SM_STATEID new, SM_EVENT e){ VOIDALL; storeHistory(__func__, onS1Enter); }

static void onS1Leave(SM_STATEID old, SM_STATEID new, SM_EVENT e){ VOIDALL; storeHistory(__func__, onS1Leave); }

static void onS2Leave(SM_STATEID old, SM_STATEID new, SM_EVENT e){ VOIDALL; storeHistory(__func__, onS2Leave); }

static void onS3Enter(SM_STATEID old, SM_STATEID new, SM_EVENT e){ VOIDALL; storeHistory(__func__, onS3Enter); }

static void onE1S1(SM_STATEID old, SM_STATEID new, SM_EVENT e){ VOIDALL; storeHistory(__func__, onE1S1); }
static void onE2S1(SM_STATEID old, SM_STATEID new, SM_EVENT e){ VOIDALL; storeHistory(__func__, onE2S1); }
static void onE3S1(SM_STATEID old, SM_STATEID new, SM_EVENT e){ VOIDALL; storeHistory(__func__, onE3S1); }

static void onE4S2(SM_STATEID old, SM_STATEID new, SM_EVENT e){ VOIDALL; storeHistory(__func__, onE4S2); }
static void onE4S3(SM_STATEID old, SM_STATEID new, SM_EVENT e){ VOIDALL; storeHistory(__func__, onE4S3); }
static void onE4S4(SM_STATEID old, SM_STATEID new, SM_EVENT e){	VOIDALL; storeHistory(__func__, onE4S4); }

static void onE5S4(SM_STATEID old, SM_STATEID new, SM_EVENT e)
{
	storeHistory(__func__, onE5S4);
	TEST_ASSERT_EQUAL(S4, old);
	TEST_ASSERT_EQUAL(S5, new);
	TEST_ASSERT_EQUAL(E5, e);
	SM_Event(&sm, E6);
}

static void onE6S5(SM_STATEID old, SM_STATEID new, SM_EVENT e)
{
	storeHistory(__func__, onE6S5);
	TEST_ASSERT_EQUAL(S5, old);
	TEST_ASSERT_EQUAL(S6, new);
	TEST_ASSERT_EQUAL(E6, e);

	SM_Event(&sm, E7);
}

static void onE7S6(SM_STATEID old, SM_STATEID new, SM_EVENT e)
{
	storeHistory(__func__, onE7S6);
	TEST_ASSERT_EQUAL(S6, old);
	TEST_ASSERT_EQUAL(S1, new);
	TEST_ASSERT_EQUAL(E7, e);

}

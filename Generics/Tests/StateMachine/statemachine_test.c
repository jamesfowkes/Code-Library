#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

#include "unity.h"

#define SM_PRIVATE_ACCESS
#include "../../ringbuf.h"
#include "../../statemachine.h"
#include "../../statemachinemanager.h"

void setUp(void)
{

}

void tearDown(void)
{

}

void test_GetStateMachinePointers(void)
{
	printf("Size of state machine = %d, size of event = %d\n", sizeof(STATE_MACHINE_INTERNAL), sizeof(SM_EVENT));
	TEST_ASSERT( SMM_Config(2, 20) );
}


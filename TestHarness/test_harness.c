/*
 * Standard Library Includes
 */
 
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

/*
 * Utility Library Includes
 */

#include "util_macros.h"

/* 
 * Test Harness Library Includes
 */

#include "test_harness.h"

/*
 * Local Variables
 */
 
static char s_lastResultString[200];

/*
 * Public Function Definitions
 */
 
bool TestHarnessRunNext(TEST_GROUP * testGroup)
{

	TEST * thisTest;
	
	if (testGroup->lastTestRan >= 0)
	{
		thisTest = &(testGroup->tests[testGroup->lastTestRan]);
		
		thisTest->bResult = true;
		thisTest->resultFn(thisTest);
		
		if (!thisTest->bResult)
		{
			printf(" FAIL: %s.\n", s_lastResultString);
			exit(1);
		}
		else
		{
			printf(" PASS.\n");
		}
	}
		
	testGroup->lastTestRan++;
	
	if (testGroup->testCount == (uint32_t)testGroup->lastTestRan) { return false; }
	
	thisTest = &(testGroup->tests[testGroup->lastTestRan]);
	
	printf("Running test '%s'...", thisTest->name);
	thisTest->testFn();
	
	return true;
}

void TestAssertEqual(TEST * test, int32_t a, int32_t b, char * pa, char * pb)
{
	test->bResult = (a == b);
	
	if (!test->bResult)
	{
		sprintf(s_lastResultString, "%s != %s (%d != %d)", pa, pb, a, b);
	}
}
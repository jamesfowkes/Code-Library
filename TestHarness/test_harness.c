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
 * Public Function Definitions
 */
 
bool TestHarnessRunNext(TEST_GROUP * testGroup)
{

	TEST * thisTest;
	
	if (testGroup->lastTestRan >= 0)
	{
		thisTest = &(testGroup->tests[testGroup->lastTestRan]);
		
		if (!thisTest->resultFn())
		{
			printf(" FAIL.\n");
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

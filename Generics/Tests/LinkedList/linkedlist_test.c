#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

#include "unity.h"

#include "../../linkedlist.h"

struct test_struct
{
	LINK_NODE Node;
	uint8_t id;
	bool test;
};
typedef struct test_struct TEST_STRUCT;

static char message[100];

static LINK_NODE Head;
static uint8_t callbackCount;
static TEST_STRUCT testData[5];

void addNodes(void);
bool llCallback(LINK_NODE * node);
bool llCallbackStopEarly(LINK_NODE * node);
void setUp(void)
{
	callbackCount = 0;
	uint8_t i = 0;
	for (i = 0; i < 5; ++i)
	{
		testData[i].test = false;
		testData[i].id = i;
	}
}

void tearDown(void)
{
	printf("\n");
}

void test_AddingNodes(void)
{
	TEST_ASSERT_EQUAL( LList_Init(&Head), true);
	addNodes();
}

void addNodes(void)
{
	TEST_ASSERT_EQUAL( LList_Add(&Head, &testData[0].Node), true);
	TEST_ASSERT_EQUAL( LList_Add(&Head, &testData[1].Node), true);
	TEST_ASSERT_EQUAL( LList_Add(&Head, &testData[2].Node), true);
	TEST_ASSERT_EQUAL( LList_Add(&Head, &testData[3].Node), true);
	TEST_ASSERT_EQUAL( LList_Add(&Head, &testData[4].Node), true);
}

void test_CountingNodes(void)
{
	TEST_ASSERT_EQUAL( LList_ItemCount(&Head), 5);
}

void test_RemovingHeadNodes(void)
{
	TEST_ASSERT_EQUAL( LList_Remove(&Head, &testData[0].Node), true);
	LList_Traverse(&Head, llCallback); printf(" ");
	TEST_ASSERT_EQUAL( LList_ItemCount(&Head), 4);
	TEST_ASSERT_EQUAL( LList_Remove(&Head, &testData[1].Node), true);
	LList_Traverse(&Head, llCallback); printf(" ");
	TEST_ASSERT_EQUAL( LList_ItemCount(&Head), 3);
	TEST_ASSERT_EQUAL( LList_Remove(&Head, &testData[2].Node), true);
	LList_Traverse(&Head, llCallback); printf(" ");
	TEST_ASSERT_EQUAL( LList_ItemCount(&Head), 2);
	TEST_ASSERT_EQUAL( LList_Remove(&Head, &testData[3].Node), true);
	LList_Traverse(&Head, llCallback); printf(" ");
	TEST_ASSERT_EQUAL( LList_ItemCount(&Head), 1);
	TEST_ASSERT_EQUAL( LList_Remove(&Head, &testData[4].Node), true);
	LList_Traverse(&Head, llCallback); printf(" ");
	TEST_ASSERT_EQUAL( LList_ItemCount(&Head), 0);
	
	addNodes();
}

void test_RemovingMiddleNodes(void)
{
	TEST_ASSERT_EQUAL( LList_Remove(&Head, &testData[3].Node), true);
	LList_Traverse(&Head, llCallback); printf(" ");
	TEST_ASSERT_EQUAL( LList_ItemCount(&Head), 4);
	TEST_ASSERT_EQUAL( LList_Remove(&Head, &testData[2].Node), true);
	LList_Traverse(&Head, llCallback); printf(" ");
	TEST_ASSERT_EQUAL( LList_ItemCount(&Head), 3);
	TEST_ASSERT_EQUAL( LList_Remove(&Head, &testData[1].Node), true);
	LList_Traverse(&Head, llCallback); printf(" ");
	TEST_ASSERT_EQUAL( LList_ItemCount(&Head), 2);
	TEST_ASSERT_EQUAL( LList_Remove(&Head, &testData[4].Node), true);
	LList_Traverse(&Head, llCallback); printf(" ");
	TEST_ASSERT_EQUAL( LList_ItemCount(&Head), 1);
	TEST_ASSERT_EQUAL( LList_Remove(&Head, &testData[0].Node), true);
	LList_Traverse(&Head, llCallback); printf(" ");
	TEST_ASSERT_EQUAL( LList_ItemCount(&Head), 0);
	TEST_ASSERT_EQUAL(Head.next, NULL);
	addNodes();
}

void test_RemovingTailNodes(void)
{
	TEST_ASSERT_EQUAL( LList_Remove(&Head, &testData[4].Node), true);
	LList_Traverse(&Head, llCallback); printf(" ");
	TEST_ASSERT_EQUAL( LList_ItemCount(&Head), 4);
	TEST_ASSERT_EQUAL( LList_Remove(&Head, &testData[3].Node), true);
	LList_Traverse(&Head, llCallback); printf(" ");
	TEST_ASSERT_EQUAL( LList_ItemCount(&Head), 3);
	TEST_ASSERT_EQUAL( LList_Remove(&Head, &testData[2].Node), true);
	LList_Traverse(&Head, llCallback); printf(" ");
	TEST_ASSERT_EQUAL( LList_ItemCount(&Head), 2);
	TEST_ASSERT_EQUAL( LList_Remove(&Head, &testData[1].Node), true);
	LList_Traverse(&Head, llCallback); printf(" ");
	TEST_ASSERT_EQUAL( LList_ItemCount(&Head), 1);
	TEST_ASSERT_EQUAL( LList_Remove(&Head, &testData[0].Node), true);
	LList_Traverse(&Head, llCallback); printf(" ");
	TEST_ASSERT_EQUAL( LList_ItemCount(&Head), 0);
	
	addNodes();
}

void test_TraversingNodes(void)
{
	LList_Traverse(&Head, llCallback);
	TEST_ASSERT_EQUAL(callbackCount, 5);
	TEST_ASSERT_EQUAL(testData[0].test, true);
	TEST_ASSERT_EQUAL(testData[1].test, true);
	TEST_ASSERT_EQUAL(testData[2].test, true);
	TEST_ASSERT_EQUAL(testData[3].test, true);
	TEST_ASSERT_EQUAL(testData[4].test, true);
	
	setUp();

	LList_Traverse(&Head, llCallbackStopEarly);
	TEST_ASSERT_EQUAL(callbackCount, 3);
	TEST_ASSERT_EQUAL(testData[0].test, true);
	TEST_ASSERT_EQUAL(testData[1].test, true);
	TEST_ASSERT_EQUAL(testData[2].test, true);
	TEST_ASSERT_EQUAL(testData[3].test, false);
	TEST_ASSERT_EQUAL(testData[4].test, false);
}

bool llCallback(LINK_NODE * node)
{
	callbackCount++;
	printf("%d", ((TEST_STRUCT*)node)->id);
	((TEST_STRUCT*)node)->test = true;
	return false;
}

bool llCallbackStopEarly(LINK_NODE * node)
{
	callbackCount++;
	printf("%d", ((TEST_STRUCT*)node)->id);
	((TEST_STRUCT*)node)->test = true;
	return (callbackCount == 3);
}

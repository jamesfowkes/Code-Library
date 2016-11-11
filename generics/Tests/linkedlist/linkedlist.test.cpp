#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/extensions/HelperMacros.h>

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
void reset(void);

bool llCallback(LINK_NODE * node);
bool llCallbackStopEarly(LINK_NODE * node);
bool llCallbackRemoveOnCall(LINK_NODE * node);

class LinkedListTest : public CppUnit::TestFixture  {

	CPPUNIT_TEST_SUITE(LinkedListTest);
	CPPUNIT_TEST(test_AddingNodes);
	CPPUNIT_TEST(test_CountingNodes);
	CPPUNIT_TEST(test_RemovingHeadNodes);
	CPPUNIT_TEST(test_RemovingMiddleNodes);
	CPPUNIT_TEST(test_RemovingTailNodes);
	CPPUNIT_TEST(test_TraversingNodes);
	CPPUNIT_TEST(test_TraversingNodesStopEarly);
	CPPUNIT_TEST(test_TraversingNodesWithRemove);
	CPPUNIT_TEST_SUITE_END();

	void reset(void)
	{
		Head.next = NULL;
		uint8_t i = 0;
		for (i = 0; i < 5; ++i)
		{
			testData[i].Node.prev = NULL;
			testData[i].Node.next = NULL;
		}
	}

	void test_AddingNodes(void)
	{
		CPPUNIT_ASSERT_EQUAL( LList_Init(&Head), true);
	}

	void addNodes(void)
	{
		CPPUNIT_ASSERT_EQUAL( LList_Add(&Head, &testData[0].Node), true);
		CPPUNIT_ASSERT_EQUAL( LList_Add(&Head, &testData[1].Node), true);
		CPPUNIT_ASSERT_EQUAL( LList_Add(&Head, &testData[2].Node), true);
		CPPUNIT_ASSERT_EQUAL( LList_Add(&Head, &testData[3].Node), true);
		CPPUNIT_ASSERT_EQUAL( LList_Add(&Head, &testData[4].Node), true);
	}

	void test_CountingNodes(void)
	{
		CPPUNIT_ASSERT_EQUAL( (uint8_t)5, LList_ItemCount(&Head));
	}

	void test_RemovingHeadNodes(void)
	{
		CPPUNIT_ASSERT_EQUAL( LList_Remove(&Head, &testData[0].Node), true);
		LList_Traverse(&Head, llCallback);
		CPPUNIT_ASSERT_EQUAL( (uint8_t)4, LList_ItemCount(&Head));
		CPPUNIT_ASSERT_EQUAL( LList_Remove(&Head, &testData[1].Node), true);
		LList_Traverse(&Head, llCallback);
		CPPUNIT_ASSERT_EQUAL( (uint8_t)3, LList_ItemCount(&Head));
		CPPUNIT_ASSERT_EQUAL( LList_Remove(&Head, &testData[2].Node), true);
		LList_Traverse(&Head, llCallback);
		CPPUNIT_ASSERT_EQUAL( (uint8_t)2, LList_ItemCount(&Head));
		CPPUNIT_ASSERT_EQUAL( LList_Remove(&Head, &testData[3].Node), true);
		LList_Traverse(&Head, llCallback);
		CPPUNIT_ASSERT_EQUAL( (uint8_t)1, LList_ItemCount(&Head));
		CPPUNIT_ASSERT_EQUAL( LList_Remove(&Head, &testData[4].Node), true);
		LList_Traverse(&Head, llCallback);
		CPPUNIT_ASSERT_EQUAL( (uint8_t)0, LList_ItemCount(&Head));
	}

	void test_RemovingMiddleNodes(void)
	{
		CPPUNIT_ASSERT_EQUAL( LList_Remove(&Head, &testData[3].Node), true);
		LList_Traverse(&Head, llCallback);
		CPPUNIT_ASSERT_EQUAL( (uint8_t)4, LList_ItemCount(&Head));
		CPPUNIT_ASSERT_EQUAL( LList_Remove(&Head, &testData[2].Node), true);
		LList_Traverse(&Head, llCallback);
		CPPUNIT_ASSERT_EQUAL( (uint8_t)3, LList_ItemCount(&Head));
		CPPUNIT_ASSERT_EQUAL( LList_Remove(&Head, &testData[1].Node), true);
		LList_Traverse(&Head, llCallback);
		CPPUNIT_ASSERT_EQUAL( (uint8_t)2, LList_ItemCount(&Head));
		CPPUNIT_ASSERT_EQUAL( LList_Remove(&Head, &testData[4].Node), true);
		LList_Traverse(&Head, llCallback);
		CPPUNIT_ASSERT_EQUAL( (uint8_t)1, LList_ItemCount(&Head));
		CPPUNIT_ASSERT_EQUAL( LList_Remove(&Head, &testData[0].Node), true);
		LList_Traverse(&Head, llCallback);
		CPPUNIT_ASSERT_EQUAL( (uint8_t)0, LList_ItemCount(&Head));
		CPPUNIT_ASSERT(Head.next == NULL);
	}

	void test_RemovingTailNodes(void)
	{
		CPPUNIT_ASSERT_EQUAL( LList_Remove(&Head, &testData[4].Node), true);
		LList_Traverse(&Head, llCallback);
		CPPUNIT_ASSERT_EQUAL( (uint8_t)4, LList_ItemCount(&Head));
		CPPUNIT_ASSERT_EQUAL( LList_Remove(&Head, &testData[3].Node), true);
		LList_Traverse(&Head, llCallback);
		CPPUNIT_ASSERT_EQUAL( (uint8_t)3, LList_ItemCount(&Head));
		CPPUNIT_ASSERT_EQUAL( LList_Remove(&Head, &testData[2].Node), true);
		LList_Traverse(&Head, llCallback);
		CPPUNIT_ASSERT_EQUAL( (uint8_t)2, LList_ItemCount(&Head));
		CPPUNIT_ASSERT_EQUAL( LList_Remove(&Head, &testData[1].Node), true);
		LList_Traverse(&Head, llCallback);
		CPPUNIT_ASSERT_EQUAL( (uint8_t)1, LList_ItemCount(&Head));
		CPPUNIT_ASSERT_EQUAL( LList_Remove(&Head, &testData[0].Node), true);
		LList_Traverse(&Head, llCallback);
		CPPUNIT_ASSERT_EQUAL( (uint8_t)0, LList_ItemCount(&Head));
	}

	void test_TraversingNodes(void)
	{
		LList_Traverse(&Head, llCallback);
		CPPUNIT_ASSERT_EQUAL((uint8_t)5, callbackCount);
		CPPUNIT_ASSERT_EQUAL(testData[0].test, true);
		CPPUNIT_ASSERT_EQUAL(testData[1].test, true);
		CPPUNIT_ASSERT_EQUAL(testData[2].test, true);
		CPPUNIT_ASSERT_EQUAL(testData[3].test, true);
		CPPUNIT_ASSERT_EQUAL(testData[4].test, true);	
	}

	void test_TraversingNodesStopEarly(void)
	{
		LList_Traverse(&Head, llCallbackStopEarly);
		CPPUNIT_ASSERT_EQUAL((uint8_t)3, callbackCount);
		CPPUNIT_ASSERT_EQUAL(testData[0].test, true);
		CPPUNIT_ASSERT_EQUAL(testData[1].test, true);
		CPPUNIT_ASSERT_EQUAL(testData[2].test, true);
		CPPUNIT_ASSERT_EQUAL(testData[3].test, false);
		CPPUNIT_ASSERT_EQUAL(testData[4].test, false);
	}

	void test_TraversingNodesWithRemove(void)
	{
		LList_Traverse(&Head, llCallbackRemoveOnCall);
		CPPUNIT_ASSERT_EQUAL((uint8_t)5, callbackCount);
		CPPUNIT_ASSERT_EQUAL((uint8_t)3, LList_ItemCount(&Head));
		LList_Traverse(&Head, llCallback);
	}

	static bool llCallback(LINK_NODE * node)
	{
		callbackCount++;
		((TEST_STRUCT*)node)->test = true;
		return false;
	}

	static bool llCallbackStopEarly(LINK_NODE * node)
	{
		callbackCount++;
		((TEST_STRUCT*)node)->test = true;
		return (callbackCount == 3);
	}

	static bool llCallbackRemoveOnCall(LINK_NODE * node)
	{
		callbackCount++;
		if ((callbackCount-1) & 1)
		{
			LList_Remove(&Head, node);
		}
		return false;
		
	}

	public:
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
		reset();
		addNodes();
	}

};

int main()
{
   CppUnit::TextUi::TestRunner runner;
   
   CPPUNIT_TEST_SUITE_REGISTRATION( LinkedListTest );

   CppUnit::TestFactoryRegistry &registry = CppUnit::TestFactoryRegistry::getRegistry();

   runner.addTest( registry.makeTest() );
   runner.run();

   return 0;
}
/* AUTOGENERATED FILE. DO NOT EDIT. */

//=======Test Runner Used To Run Each Test Below=====
#define RUN_TEST(TestFunc, TestLineNum) \
{ \
  Unity.CurrentTestName = #TestFunc; \
  Unity.CurrentTestLineNumber = TestLineNum; \
  Unity.NumberOfTests++; \
  if (TEST_PROTECT()) \
  { \
      setUp(); \
      TestFunc(); \
  } \
  if (TEST_PROTECT() && !TEST_IS_IGNORED) \
  { \
    tearDown(); \
  } \
  UnityConcludeTest(); \
}

//=======Automagically Detected Files To Include=====
#include "unity.h"
#include <setjmp.h>
#include <stdio.h>

//=======External Functions This Runner Calls=====
extern void setUp(void);
extern void tearDown(void);
extern void test_AveragerInit(void);
extern void test_Averager8Running(void);
extern void test_Averager16Running(void);
extern void test_Averager32Running(void);
extern void test_Averager8Reset(void);
extern void test_Averager16Reset(void);
extern void test_Averager32Reset(void);

//=======Test Reset Option=====
void resetTest()
{
  tearDown();
  setUp();
}


//=======MAIN=====
int main(void)
{
  Unity.TestFile = "averager_test.c";
  UnityBegin();
  RUN_TEST(test_AveragerInit, 1);
  RUN_TEST(test_Averager8Running, 2);
  RUN_TEST(test_Averager16Running, 3);
  RUN_TEST(test_Averager32Running, 4);
  RUN_TEST(test_Averager8Reset, 5);
  RUN_TEST(test_Averager16Reset, 6);
  RUN_TEST(test_Averager32Reset, 7);
  return (UnityEnd());
}

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
extern void test_UnixSecondsToTime(void);
extern void test_LeapYears(void);
extern void test_DaysInMonth(void);
extern void test_TimeToUnixSeconds(void);
extern void test_IncrementSeconds(void);

//=======Test Reset Option=====
void resetTest()
{
  tearDown();
  setUp();
}


//=======MAIN=====
int main(void)
{
  Unity.TestFile = "util_time_test.c";
  UnityBegin();
  RUN_TEST(test_LeapYears, 1);
  RUN_TEST(test_DaysInMonth, 2);
  RUN_TEST(test_UnixSecondsToTime, 3);
  RUN_TEST(test_TimeToUnixSeconds, 4);
  RUN_TEST(test_IncrementSeconds, 5); 
  return (UnityEnd());
}

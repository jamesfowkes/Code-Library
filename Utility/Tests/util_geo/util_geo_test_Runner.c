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
extern void testAngleBetweenSameVectorsIsZero(void);
extern void testAngleBetweenOrthogonalVectorsIsHalfPi(void);
extern void testAngleBetweenOpposingVectorsIsPi(void);
extern void testAngleBetween45degreeVectorsIsQuarterPi(void);
extern void testLengthOfVector(void);
extern void testSubtractVectors(void);
extern void test_VectorAzimuth(void);
extern void test_VectorElevationFirstQuadrant(void);
extern void test_VectorElevationSecondQuadrant(void);
extern void test_VectorElevationThirdQuadrant(void);
extern void test_VectorElevationFourthQuadrant(void);

//=======Test Reset Option=====
void resetTest()
{
  tearDown();
  setUp();
}


//=======MAIN=====
int main(void)
{
  Unity.TestFile = "util_geo_test.c";
  UnityBegin();

  RUN_TEST(testAngleBetweenSameVectorsIsZero, 1);
  RUN_TEST(testAngleBetweenOrthogonalVectorsIsHalfPi, 2);
  RUN_TEST(testAngleBetweenOpposingVectorsIsPi, 3);
  RUN_TEST(testAngleBetween45degreeVectorsIsQuarterPi, 4);
  RUN_TEST(testLengthOfVector, 5);
  RUN_TEST(testSubtractVectors, 6);
  RUN_TEST(test_VectorAzimuth, 7);
  RUN_TEST(test_VectorElevationFirstQuadrant, 8);
  RUN_TEST(test_VectorElevationSecondQuadrant, 9);
  RUN_TEST(test_VectorElevationThirdQuadrant,10);
  RUN_TEST(test_VectorElevationFourthQuadrant,10);
  
  return (UnityEnd());
}

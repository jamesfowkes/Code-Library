#ifndef _TEST_HARNESS_H_
#define _TEST_HARNESS_H_

/*
 * Defines and Typedefs
 */
 
#define TEST_FN_NAME(x) test ## x
#define RESULT_FN_NAME(x) result ## x

#define DECLARE_TEST_FUNC(x) static void TEST_FN_NAME(x)(void)
#define DECLARE_RESULT_FUNC(x)  static void RESULT_FN_NAME(x)(TEST * test)
#define DECLARE_TEST_FUNCS(x) DECLARE_TEST_FUNC(x); DECLARE_RESULT_FUNC(x)

#define TEST_ARRAY_NAME(x) x ## Array
#define DECLARE_TEST_GROUP(x) TEST TEST_ARRAY_NAME(x)[] = 
#define END_TEST_GROUP(x) TEST_GROUP x = {TEST_ARRAY_NAME(x), N_ELE( ApplicationTestsArray ),  -1}

#define TEST_STRUCT(x) {TEST_FN_NAME(x), 1, RESULT_FN_NAME(x), true, #x}
#define TEST_STRUCT_N(x, n) {TEST_FN_NAME(x), n, RESULT_FN_NAME(x), true, #x}
#define DECLARE_TEST(x) TEST x = TEST_STRUCT(x)

#define RETURN_ON_FAIL(x) if (!x) { return false; }
#define TEST_ASSERT_EQUAL(a, b) TestAssertEqual(test, a, b, #a, #b); if(!test->bResult) { return; }

typedef struct test TEST;

typedef void (*TEST_FN)(void);
typedef void (*RESULT_FN)(TEST * test);

struct test
{	
	TEST_FN testFn;
	uint8_t n;
	RESULT_FN resultFn;
	bool bResult;
	char * name;
};

struct test_group
{
	TEST * tests;
	uint32_t testCount;
	int32_t lastTestRan;
};
typedef struct test_group TEST_GROUP;

/*
 * Public Function Prototypes
 */

bool TestHarnessRunNext(TEST_GROUP * testGroup);
void TestAssertEqual(TEST * test, int32_t a, int32_t b, char * pa, char * pb);

#endif

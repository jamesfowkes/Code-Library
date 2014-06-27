#ifndef _TEST_HARNESS_H_
#define _TEST_HARNESS_H_

/*
 * Defines and Typedefs
 */
 
#define TEST_FN_NAME(x) test ## x
#define RESULT_FN_NAME(x) result ## x

#define DECLARE_TEST_FUNC(x) static void TEST_FN_NAME(x)(void)
#define DECLARE_RESULT_FUNC(x)  static bool RESULT_FN_NAME(x)(void)
#define DECLARE_TEST_FUNCS(x) DECLARE_TEST_FUNC(x); DECLARE_RESULT_FUNC(x);

#define TEST_ARRAY_NAME(x) x ## Array
#define DECLARE_TEST_GROUP(x) TEST TEST_ARRAY_NAME(x)[] = 
#define END_TEST_GROUP(x) TEST_GROUP x = {TEST_ARRAY_NAME(x), N_ELE( ApplicationTestsArray ), -1}

#define TEST_STRUCT(x) {TEST_FN_NAME(x), RESULT_FN_NAME(x), #x}
#define DECLARE_TEST(x) TEST x = TEST_STRUCT(x)

typedef void (*TEST_FN)(void);
typedef bool (*RESULT_FN)(void);

struct test
{	
	TEST_FN testFn;
	RESULT_FN resultFn;
	char * name;
};
typedef struct test TEST;

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

#endif

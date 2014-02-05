#include <stdint.h>
#include <stdio.h>

struct test_struct
{
	uint32_t test1;
	uint32_t test2;
	uint32_t test3;
};
typedef struct test_struct TEST_STRUCT;

static TEST_STRUCT test[100];

int main(int argc, char * argv[])
{
	(void)argc;
	(void)argv;
	
	//printf("%ul\n	", sizeof uint32_t);
	printf("%u\n", sizeof(TEST_STRUCT));
	printf("%u\n", sizeof test);
	printf("%u\n", sizeof(test));
	printf("%u\n", sizeof(test)/sizeof(TEST_STRUCT));
	return 0;
}

#ifdef _AVR_ARCH_
#include <util/atomic.h>
#else
#define ATOMIC_BLOCK(x)
#define ATOMIC_RESTORESTATE
#endif

bool check_and_clear(bool& flag)
{
	bool value = false;
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
	{
		if (flag)
		{
			value = true;
			flag = false;
		}
	}
	return value;
}


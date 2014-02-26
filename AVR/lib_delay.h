#ifndef _DELAY_H_
#define _DELAY_H_

#include <util/delay.h>
#include "lib_fuses.h"

#define DELAY_US(us) \
if (FUS_IsClockDiv8Enabled()) \
{ \
	_delay_us((us+4)/8); \
} \
else \
{ \
	_delay_us(us); \
}

#define DELAY_MS(ms) \
if (FUS_IsClockDiv8Enabled()) \
{ \
	_delay_ms((ms+4)/8); \
} \
else \
{ \
	_delay_ms(ms); \
}

#endif

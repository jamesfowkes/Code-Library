#ifndef _UTIL_FIXEDPOINT_H_
#define _UTIL_FIXEDPOINT_H_

#ifdef USE_FIX16

#include "fix16.h"

#define fp_to_int(x)			fix16_to_int(x)
#define fp_from_int(x)			fix16_from_int(x)
#define fp_div(x, y)			fix16_div(x, y)
#define fp_log(x)				fix16_log(x)
#define fp_add(x, y) 			fix16_add(x, y)
#define fp_sub(x, y) 			fix16_sub(x, y)
#define fp_mul(x, y) 			fix16_mul(x, y)
#define fp_exp(x)				fix16_exp(x)
#define fp_to_str(x, buf, n)	fix16_to_str(x, buf, n)

typedef fix16_t FIXED_POINT_TYPE;

#else

#include "math.h"

#define fp_to_int(x) 			((int32_t)(x))
#define fp_from_int(x)			((float)(x))
#define fp_div(x, y)			(x / y)
#define fp_log(x)				log(x)
#define fp_add(x, y)			(x + y)
#define fp_sub(x, y)			(x - y)
#define fp_mul(x, y)			(x * y)
#define fp_exp(x)				(exp(x))
#define fp_to_str(x, buf, n)	sprintf(buf, "%.*f", n, x)

typedef float FIXED_POINT_TYPE;

#endif

#endif



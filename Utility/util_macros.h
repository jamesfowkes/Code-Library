#ifndef _UTIL_MACROS_H_
#define _UTIL_MACROS_H_

#define min(a,b) (((a) < (b)) ? (a) : (b))

#define max(a,b) (((a) > (b)) ? (a) : (b))

#define inrange(n, lo, hi) ((lo <= n) && (hi >= n))

#define lastinloop(i, loopmax) ((i == (loopmax - 1))

#define incrementwithrollover(var, max) (var = (var < max) ? var + 1 : 0)

#endif

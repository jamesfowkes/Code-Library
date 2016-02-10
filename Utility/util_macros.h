#ifndef _UTIL_MACROS_H_
#define _UTIL_MACROS_H_

#define min(a,b) (((a) < (b)) ? (a) : (b))

#define max(a,b) (((a) > (b)) ? (a) : (b))

#define inrange(n, lo, hi) ((lo <= n) && (hi >= n))

#define lastinloop(i, loopmax) ((i == (loopmax - 1)))

#define incrementwithrollover(var, max) (var = (var < max) ? var + 1 : 0)
#define decrementwithrollover(var, max) (var = (var > 0) ? var - 1 : max)

#define set(reg, bit) (reg |= (1 << bit))
#define clr(reg, bit) (reg &= ~(1 << bit))

#define div_round_pos(x, y) (((x) + ((y)/2))/(y))
#define div_round_neg(x, y) (((x) - ((y)/2))/(y))

#define div_round(x, y) ((x == 0) ? 0 : ((x > 0) ? div_round_pos(x, y) : div_round_neg(x, y)))

#define N_ELE(x) (sizeof(x)/sizeof(x[0]))

#endif

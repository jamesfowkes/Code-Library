#ifndef _ARDUINO_EXTRA_H_
#define _ARDUINO_EXTRA_H_

#ifndef max
template <class T>
static inline T max(T a, T b) {
 return (a>b?a:b);
}
#endif

#ifndef min
template <class T>
static inline T min(T a, T b) {
 return (a<b?a:b);
}
#endif

#ifndef abs
template <class T>
static inline T abs(T a) {
 return (a<0?-a:a);
}
#endif

// Number of elements in array x
#define N_ELE(x) (sizeof(x)/sizeof(x[0]))

// True if x will index the last element of an N sized array, or will be the last run of an N-based for loop
#define islast(x, N) (x == (N-1))

// Maintains function signature with unused parameters
#define	_UNUSED_(type, unique_id)	type Dummy##unique_id

#endif

#ifndef _COUNTERS_H_
#define _COUNTERS_H_

template <typename T>
T increment_with_rollover(T x, T max)
{
	return (x == max) ? 0 : (x+1);
}

template <typename T>
bool decrement_reset_at_zero(T& count, const T limit)
{
	if (count > 0) { count--; }	
	
	bool reset = (count == 0);

	if (reset) { count = limit; }

	return reset;
}

template <typename T>
bool decrement_with_limit(T& value, const T limit)
{
	if (value > limit) { value--; }
	return value == limit;
}

template <typename T>
bool increment_with_limit(T& value, const T limit)
{
	if (value < limit) { value++; }
	return value == limit;
}

#endif

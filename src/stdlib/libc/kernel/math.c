/*
 * math.c
 *
 *  Created on: Jan 7, 2018
 *      Author: seeseemelk
 */
#include "math.h"

size_t ceilg(size_t n, size_t granularity)
{
	return ((n - 1) / granularity + 1) * granularity;
}

size_t floorg(size_t n, size_t granularity)
{
	return (n / granularity) * granularity;
}

size_t ceildiv(size_t a, size_t b)
{
	return (a == 0) ? 0 : ((a - 1) / b + 1);
}

/*
 * Taken from https://graphics.stanford.edu/~seander/bithacks.html#IntegerLog
 */
size_t log2(size_t v)
{
	const unsigned int b[] =
	{ 0x2, 0xC, 0xF0, 0xFF00, 0xFFFF0000 };
	const unsigned int S[] =
	{ 1, 2, 4, 8, 16 };
	int i;

	register unsigned int r = 0; // result of log2(v) will go here
	for (i = 4; i >= 0; i--) // unroll for speed...
	{
		if (v & b[i])
		{
			v >>= S[i];
			r |= S[i];
		}
	}
	return r;
}

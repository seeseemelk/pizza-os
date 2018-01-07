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

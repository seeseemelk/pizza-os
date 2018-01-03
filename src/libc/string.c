/*
 * string.c
 *
 *  Created on: Jan 3, 2018
 *      Author: seeseemelk
 */
#include "string.h"

void* memcpy(void* dest, void* src, size_t num)
{
	char* c_dest = (char*) dest;
	char* c_src = (char*) src;

	if (src > dest)
	{
		for (size_t i = 0; i < num; i++)
			c_dest[i] = c_src[i];
	}
	else
	{
		for (size_t i = num-1; i >= 0; i--)
			c_dest[i] = c_src[i];
	}
	return dest;
}

void* memset(void* ptr, int value, size_t num)
{
	char* c_ptr = (char*) ptr;
	for (size_t i = 0; i < num; i++)
		c_ptr[i] = (char) value;
	return ptr;
}


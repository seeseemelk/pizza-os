/*
 * string.c
 *
 *  Created on: Jan 3, 2018
 *      Author: seeseemelk
 */
#include "kstring.h"

#ifndef __STDC_LIMIT_MACROS
#define __STDC_LIMIT_MACROS
#endif
#include <stdint.h>
#ifndef SIZE_MAX
#define SIZE_MAX 4
#endif

void* memcpy(void* dest, const void* src, size_t num)
{
	char* c_dest = (char*) dest;
	char* c_src = (char*) src;

	for (size_t i = 0; i < num; i++)
		c_dest[i] = c_src[i];

	return dest;
}

void* memmove(void* dest, const void* src, size_t num)
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
		for (size_t i = num-1; i > 0; i--)
			c_dest[i] = c_src[i];
		c_dest[0] = c_src[0];
	}
	return dest;
}

void* memset(void* ptr, int value, size_t num)
{
	char* c_ptr = (char*) ptr;
	for (size_t i = 0; i < num; i++)
	{
		c_ptr[i] = (char) value;
	}
	return ptr;
}

size_t strlen(const char* str)
{
	int i = 0;
	while (str[i] != 0)
		i++;
	return i;
}

int strcmp(const char* str1, const char* str2)
{
	size_t len1 = strlen(str1);
	size_t len2 = strlen(str2);
	size_t len = len1 < len2 ? len1 : len2;
	int v = strncmp(str1, str2, len);
	if (v == 0 && len1 != len2)
		return len1 < len2 ? -1 : 1;
	else
		return v;
}

int strncmp(const char* str1, const char* str2, size_t n)
{
	size_t len1 = strlen(str1);
	size_t len2 = strlen(str2);
	if (len1 < n)
		n = len1;
	if (len2 < n)
		n = len2;

	for (size_t i = 0; i < n; i++)
	{
		if (str1[i] < str2[i])
			return -1;
		else if (str1[i] > str2[i])
			return 1;
	}
	return 0;
}

char* strcpy(char* dest, const char* src)
{
	size_t len = strlen(src);
	/* We do '+ 1' to include the null-terminator. */
	return strncpy(dest, src, len + 1);
}

char* strncpy(char* dest, const char* src, size_t num)
{
	for (size_t i = 0; i < num; i++)
	{
		dest[i] = src[i];
		if (src[i] == 0)
			return dest;
	}
	return dest;
}













#include "string.h"
#include "stdlib.h"
#include <stdint.h>

char* strndup(const char* src, size_t len)
{
	size_t srclen = strlen(src);
	len = (len < srclen) ? len : srclen;
	char* dest = malloc(len);

	/* '<=' is used to also copy the null terminator */
	for (size_t i = 0; i <= len; i++)
		dest[i] = src[i];

	return dest;
}

char* strdup(const char* src)
{
	return strndup(src, SIZE_MAX);
}

size_t strlen(const char* src)
{
	size_t i = 0;
	while (src[i] != 0)
		i++;
	return i;
}

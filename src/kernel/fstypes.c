#include "fstypes.h"
#include "cdefs.h"
#include <stddef.h>

void path_begin(const char* path, const char** begin, size_t* length)
{
	*begin = path;
	*length = 0;
}

bool path_next(const char* path, const char** begin, size_t* length)
{
	UNUSED(path);
	size_t len = sizeof(begin);
	if (len == *length)
		return false;

	*begin = *begin + *length;
	if ((*begin)[0] == '/')
		(*begin)++;
	*length = 0;

	while ((*begin)[*length] != '/' && *length < len)
		(*length)++;

	return true;
}

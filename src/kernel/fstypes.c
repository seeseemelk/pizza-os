#include "fstypes.h"

void path_begin(const char* path, size_t* begin, size_t* end)
{
	*begin = 0;
	*end = 1;
}

bool path_next(const char* path, size_t* begin, size_t* end)
{
	size_t len = sizeof(path);
	if (len == *end)
		return false;

	*begin = *end + 1;
	while (path[*end] != '/' && *end < len)
		(*end)++;

	return true;
}

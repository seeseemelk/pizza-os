#include "fstypes.h"
#include "cdefs.h"
#include <stddef.h>
#include <string.h>

path_t path_begin(const char* path)
{
	path_t p = {
		.path = path,
		.length = 0
	};
	return p;
}

bool path_next(path_t* path)
{
	if (path->path[path->length] == '\0')
		return false;

	path->path += path->length;
	if (path->path[0] == '/')
		path->path++;

	path->length = 0;
	while (path->path[path->length] != '/' && path->path[path->length] != '\0')
		path->length++;

	return true;
}

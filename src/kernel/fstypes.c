#include "fstypes.h"
#include "cdefs.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

path_t path_begin(const char* path)
{
	path_t p = {
		.path = path,
		.length = 0
	};
	if (p.path[0] == '/')
		p.path++;
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

void path_store(path_t* path, char* buf)
{
	strncpy(buf, path->path, path->length - 1);
	buf[path->length] = '\0';
}

char* path_parent(const char* path)
{
	char* parent = strdup(path);
	size_t end = strlen(parent);

	/* If empty string, return empty string */
	if (end == 0)
		return parent;

	/* Remove the null terminator */
	end--;

	/* Remove a trailing '/' */
	if (parent[end] == '/')
		end--;

	/* Find the last '/' */
	while (parent[end] != '/')
		end--;

	/* Null terminate the string */
	if (end > 0) /* We didn't reach just the root directory. */
		parent[end] = '\0';
	else /* We reached just the root directory. */
		parent[end+1] = '\0';

	return parent;
}

char* path_filename(const char* path)
{
	size_t end = strlen(path);
	/* Return an empty string if path is empty or contains a sole '/' */
	if (end <= 1)
	{
		char* buf = malloc(1);
		buf[0] = '\0';
		return buf;
	}

	/* Remove trailing null terminator */
	end--;

	/* Remove a trailing '/' */
	if (path[end] == '/')
		end--;
	size_t start = end;

	/* Find the position of the '/' right before the filename */
	while (path[start-1] != '/')
		start--;

	/* Allocate a new string to copy it into. */
	size_t len = (end - start) + 2; /* '+ 2' because we need (end + 1) and a null terminator */
	char* buf = malloc(len);
	strncpy(buf, path+start, len-1);

	/* Null terminate the string */
	buf[len-1] = '\0';
	return buf;
}











/*
 * fstypes.h
 *
 *  Created on: May 23, 2018
 *      Author: seeseemelk
 */

#ifndef FSTYPES_H_
#define FSTYPES_H_

#include <stddef.h>
#include <stdbool.h>
#include <string.h>

typedef size_t DIR;
typedef size_t FILE;

typedef enum
{
	FDIR, FFILE
} file_t;

typedef struct stat_t
{
	//device_t* dev;
	//ino_t ino;
	file_t type;
} stat_t;

typedef struct
{
	//devi_t dev;
	//ino_t inode;
	file_t type;
	char name[256];
} dirent_t;

typedef struct
{
	const char* path;
	size_t length;
} path_t;

typedef enum
{
	FSEEK_REL, FSEEK_BEGIN, FSEEK_END
} fseek_t;

/**
 * Initialises the `begin` and `end` parameters for use with `path_next`.
 */
path_t path_begin(const char* path);

/**
 * Walks over each part in a path string.
 */
bool path_next(path_t* path);

/**
 * Stores the name of the currently selected part of the path
 * in a buffer. The buffer will always be `null` terminated.
 * Note that the buf does need to be at least path->length in size.
 */
void path_store(path_t* path, char* buf);

/**
 * Gets the parent directory of a path.
 * Returns a `null` terminated string with no leading '/' at the end.
 * Note that the returned string has to be freed manually.
 */
char* path_parent(const char* path);

/**
 * Similar to `path_parent`, but gets the filename instead of the
 * parent directory.
 * Will never contain any '/' whatsoever.
 * Note that the returned string has to be freed manually.
 */
char* path_filename(const char* path);

/**
 * Compares a string to a path.
 * Usable as a shorthand for `strncmp` when using paths.
 */
static inline int pcmp(const char* str, path_t* path)
{
	return strncmp(str, path->path, path->length);
}

#endif /* FSTYPES_H_ */

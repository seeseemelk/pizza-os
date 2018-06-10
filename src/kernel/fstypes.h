/*
 * fstypes.h
 *
 *  Created on: May 23, 2018
 *      Author: seeseemelk
 */

#ifndef FSTYPES_H_
#define FSTYPES_H_

#include "config.h"
#include "devices.h"
#include <stddef.h>
#include <stdbool.h>
#include <string.h>

typedef size_t DIR;
typedef size_t FILE;

typedef enum
{
	FDIR, FFILE, FCHAR, FBLOCK
} file_t;

typedef enum
{
	SEEK_SET, SEEK_CUR, SEEK_END
} seek_t;

typedef struct stat_t
{
	device_t* device;
	file_t type;
	size_t size;
	size_t blocks;
	size_t block_size;
	char name[MAX_FILENAME];
} stat_t;

typedef struct
{
	file_t type;
	char name[MAX_FILENAME];
} dirent_t;

typedef struct
{
	const char* path;
	size_t length;
} path_t;

typedef enum
{
	O_READ = 0x01,
	O_WRITE = 0x02,
	O_APPEND = 0x04,
	O_CREATE = 0x08
} mode_t;

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

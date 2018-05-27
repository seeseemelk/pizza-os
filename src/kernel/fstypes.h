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

/**
 * Initialises the `begin` and `end` parameters for use with `path_next`.
 */
void path_begin(const char* path, const char** begin, size_t* length);

/**
 * Walks over each part in a path string.
 */
bool path_next(const char* path, const char** begin, size_t* length);

#endif /* FSTYPES_H_ */

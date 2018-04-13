/**
 * @file
 * Contains functions related to filesystem management.
 * vhs.h
 *
 *  Created on: Apr 11, 2018
 *      Author: seeseemelk
 */

#ifndef VFS_H_
#define VFS_H_

#include <stdbool.h>
#include <devices.h>

typedef struct
{
	const char name[256];
	unsigned int itag;
} tag_t;

typedef struct
{
	const char name[256];
	unsigned int itag[16];
} file_t;

typedef struct
{
	unsigned int itag[16];
} tagged_t;

/**
 * Initialises the VFS.
 */
void vfs_init();

#endif /* VFS_H_ */

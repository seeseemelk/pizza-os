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

typedef struct devi_t
{
	unsigned short dev_major;
	unsigned short dev_minor;
} devi_t;

typedef unsigned int ino_t;

typedef enum
{
	EDIRECTORY, EFILE
} file_t;

typedef struct stat_t
{
	devi_t dev;
	ino_t ino;
	file_t type;
} stat_t;

typedef struct mount_t
{
	device_t* dev;
	const char* path;
} mount_t;

typedef struct DIR
{
	devi_t dev;
	ino_t inode;
	void* dev_it;
} DIR;

typedef struct dirent_t
{
	devi_t dev;
	ino_t inode;
	file_t type;
	char name[256];
} dirent_t;

/**
 * Initialises the VFS.
 */
void vfs_init();

#endif /* VFS_H_ */

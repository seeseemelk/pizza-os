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

typedef enum file_t file_t;

/**
 * Describes different types of files.
 */
enum file_t
{
	FILE,
	DIRECTORY
};

typedef struct stat_t stat_t;

/**
 * Contains information about a file.
 */
struct stat_t
{
	const char* filename; /*< The name of the file. */
	unsigned long long size; /*< The size of the file. */
	unsigned int inode; /*< The inode number of the file. This is unique per device */
	file_t type;
	device* dev;
};

/**
 * Reads information about a file.
 * @param path The path of the file.
 * @param stat The `stat_t` to store the file's information in.
 * @return `0` on success, something else when an error occurred.
 */
int vfs_stat(const char* path, stat_t* stat);

/**
 * Initialises the VFS.
 */
void vfs_init();

#endif /* VFS_H_ */

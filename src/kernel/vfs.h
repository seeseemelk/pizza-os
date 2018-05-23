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

#include "bus/filesystem.h"
#include "fstypes.h"
#include <stdbool.h>
#include <devices.h>

/**
 * Initialises the VFS.
 */
void vfs_init();
void vfs_mount(const char* path, filesystem_t* fs);
DIR vfs_open_dir(const char* path);
void vfs_close_dir(DIR dir);
void vfs_next_dir(DIR dir, dirent_t* dirent);
void vfs_stat(const char* path);

#endif /* VFS_H_ */


























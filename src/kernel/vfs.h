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
void vfs_mount_direct(const char* path, filesystem_t* fs);
void vfs_mount(const char* path, const char* fs, const char* block, int argc, const char** argv);

bool vfs_rm(const char* path);
bool vfs_stat(const char* path, stat_t* stat);
void vfs_mkdir(const char* path);
void vfs_mkblock(const char* path, device_t* dev);
void vfs_mkchar(const char* path, device_t* dev);

DIR vfs_open_dir(const char* path);
void vfs_close_dir(DIR dir);
bool vfs_next_dir(DIR dir, dirent_t* dirent);

FILE vfs_open_file(const char* path, mode_t mode);
void vfs_close_file(FILE file);
size_t vfs_read_file(FILE file, char* buf, size_t len);
size_t vfs_write_file(FILE file, const char* buf, size_t len);

#endif /* VFS_H_ */


























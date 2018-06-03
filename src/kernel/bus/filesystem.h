/*
 * vfs.h
 *
 *  Created on: May 23, 2018
 *      Author: seeseemelk
 */

#ifndef BUS_FILESYSTEM_H_
#define BUS_FILESYSTEM_H_

#include "devices.h"
#include "fstypes.h"
#include <stdbool.h>

/**
 * The `filesystem_t` bus can be used to register a mounted filesystem.
 * It should be registered by a device as a bus of type `FILESYSTEM`
 */
typedef struct
{
	device_t* dev;
	int(*get_inode)(device_t* dev, const char* path);
	void(*free_inode)(device_t* dev, int inode);

	int(*mkdir)(device_t* dev, int parent, const char* name);
	int(*mkfile)(device_t* dev, int parent, const char* name);
	int(*mklink)(device_t* dev, int parent, const char* name, const char* target);

	void(*rm)(device_t* dev, int inode);
	void(*stat)(device_t* dev, int inode, stat_t* stat);

	void*(*dir_open)(device_t* dev, int inode);
	void(*dir_close)(device_t* dev, void* dirit);
	bool(*dir_next)(device_t* dev, void* dirit, dirent_t* dirent);

	void*(*file_open)(device_t* dev, int inode, mode_t mode);
	void(*file_close)(device_t* dev, void* fd);
	size_t(*file_read)(device_t* dev, void* fd, char* buf, size_t amount);
	size_t(*file_write)(device_t* dev, void* fd, const char* buf, size_t amount);
	int(*file_seek)(device_t* dev, void* fd, int amount, fseek_t relative);
} filesystem_t;

/**
 * The `filesystem_t` bus can be used to allow the system to mount a filesystem.
 * It should be registered by a module as a bus of type `FILESYSTEM`
 */
typedef struct
{
	module_t* mod;
	filesystem_t*(*mount)(module_t* mod, const char* path, int argc, const char** argv);
	bool(*can_mount)(module_t* mod, const char* path, int argc, const char** argv);
} filesystem_mount_t;

#endif /* BUS_FILESYSTEM_H_ */

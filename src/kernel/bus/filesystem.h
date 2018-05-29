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

typedef struct
{
	device_t* dev;
	int(*get_inode)(device_t* dev, const char* path);
	void(*free_inode)(device_t* dev, int inode);

	void(*mkdir)(device_t* dev, int parent, const char* name);
	void(*mkfile)(device_t* dev, int parent, const char* name);
	void(*mklink)(device_t* dev, int parent, const char* name, const char* target);

	void(*rm)(device_t* dev, int inode);
	void(*stat)(device_t* dev, int inode);

	void*(*dir_open)(device_t* dev, int inode);
	void(*dir_close)(device_t* dev, void* dirit);
	void(*dir_next)(device_t* dev, void* dirit, dirent_t* dirent);

	void*(*file_open)(device_t* dev, int inode);
	void(*file_close)(device_t* dev, void* fd);
	int(*file_read)(device_t* dev, void* fd, char* buf, int amount);
	int(*file_write)(device_t* dev, void* fd, char* buf, int amount);
	int(*file_seek)(device_t* dev, void* fd, int amount, fseek_t relative);
} filesystem_t;

#endif /* BUS_FILESYSTEM_H_ */

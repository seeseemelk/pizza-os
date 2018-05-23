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
	void*(*open_dir)(device_t* dev, int inode);
	void(*close_dir)(device_t* dev, void* dirit);
	void(*dir_next)(device_t* dev, void* dirit, dirent_t* dirent);
	void(*stat)(device_t* dev, int inode);
} filesystem_t;

#endif /* BUS_FILESYSTEM_H_ */

#include "dev/tmpfs.h"
#include "bus/filesystem.h"
#include "vfs.h"
#include "devices.h"
#include "cdefs.h"
#include "config.h"
#include "kernel.h"
#include "collections/list.h"
#include "fstypes.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FS(dev) ((fs_t*) dev)

// Contains the inode structure
typedef struct
{
	int id;
	char name[16];
	list_t* ref;
	file_t type;
} inode_t;

// Contains the device structure
typedef struct
{
	device_t dev;
	filesystem_t bus;
	inode_t root;
} fs_t;

module_t mod;
fs_t dev;

/**
 * Takes an inode and searches for a child entry in that
 * inode that has a name equal to path.
 */
inode_t* tmpfs_find_subdir(inode_t* inode, path_t* path)
{
	list_t* children = inode->ref;
	size_t length = list_size(children);

	inode_t* child;
	for (size_t i = 0; i < length; i++)
	{
		child = list_get(children, i);
		if (pcmp(child->name, path) == 0)
			return child;
	}
	kernel_panic("Inode not found");
	return NULL;
}

/**
 * Turns a path into an inode number.
 */
int tmpfs_get_inode(device_t* dev, const char* path)
{
	fs_t* fs = FS(dev);
	path_t p = path_begin(path);

	inode_t* inode = &fs->root;
	while (path_next(&p))
	{
		inode = tmpfs_find_subdir(inode, &p);
	}
	return inode->id;
}

filesystem_t* tmpfs_init()
{
	dev.bus.get_inode = tmpfs_get_inode;

	dev.root.ref = list_new();

	module_register(&mod, "tmpfs", NULL, NULL);
	device_register(&dev.dev, &mod);
	device_register_bus(&dev.dev, FILESYSTEM, &dev.bus);
	return &dev.bus;
}













#include "dev/tmpfs.h"
#include "bus/filesystem.h"
#include "vfs.h"
#include "devices.h"
#include "cdefs.h"
#include "config.h"
#include "kernel.h"
#include "collections/list.h"
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

int tmpfs_get_inode(device_t* dev, const char* path)
{
	fs_t* fs = FS(dev);
	size_t start, end;
	path_begin(path, &start, &end);

	inode_t* inode = &fs->root;
	while (path_next_part(path, &start, &end) != 0)
	{
		inode = tmpfs_find_subdir(inode, path, start, end);
	}
	return inode->id;
}

void tmpfs_init()
{
	dev.bus.get_inode = tmpfs_get_inode;

	dev.root.ref = list_new();

	module_register(&mod, "tmpfs", NULL, NULL);
	device_register(&dev.dev, &mod);
	device_register_bus(&dev.dev, FILESYSTEM, &dev.bus);
}













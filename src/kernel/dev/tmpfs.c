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

/* A general node structure. */
typedef struct
{
	int id;
	char name[16];
	file_t type;
} node_t;

/* Node structure for directories. */
typedef struct
{
	node_t node;
	/*
	 * Contains a list to all the inodes that are
	 * in this directory.
	 */
	list_t* ref;
} dirnode_t;

/* Node structure for files. */
typedef struct
{
	node_t node;
	/* Contains pointers to 512-byte buffers that
	 * hold the file data.
	 */
	list_t* contents;
} filenode_t;

// Contains the device structure
typedef struct
{
	device_t dev;
	filesystem_t bus;
	node_t root;
	/* Contains the list of all inodes in numeric order
	 * The inodes are indexed by their inode number.
	 * When inodes are free, they are set to `NULL`.
	 * `NULL` inodes can be reused.
	 */
	list_t* nodes; 
	size_t freeNodes;
} fs_t;

static module_t mod;
static fs_t dev;

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
 * This inode number can be used to directly
 * index into the fs->nodes list.
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

/**
 * Finds a free unused inode in the device's
 * node list.
 */
int tmpfs_find_unused_inode(device_t* device)
{
	if (device->freeNodes > 0)
	{
		size_t index = 0;
		size_t size = list_size(device->nodes);
		for (size_t i = 0; i < size; i++)
		{
			if (list_get(i) == NULL)
			{
				index = i;
				break;
			}
		}
		return index;
	}
	else
	{
		list_add(device->nodes, NULL);
		return list_size(device->nodes);
	}
}

/**
 * Stores a newly generated node into the device's
 * node list.
 */
void tmpfs_store_node(device_t* device, node_t* child, const char* name)
{
	/* Find a free spot in the list */
}

void tmpfs_free_inode(device_t* dev, int inode)
{
	fs_t* fs = FS(dev);
	node_t* node = list_get(fs->nodes, inode);
	free(node);
	list_set(fs->nodes, inode, NULL);
}

void tmpfs_mkdir(device_t* dev, int parentInode, const char* name)
{
	fs_t* fs = FS(dev);
	node_t* parent = list_get(fs->nodes, inode);
	dirnode_t* child = malloc(sizeof(dirnode_t));
	list_add(parent->refs, child);
	tmpfs_store_node(child, name);
	return child->id;
}

filesystem_t* tmpfs_init()
{
	dev.bus.get_inode = tmpfs_get_inode;

	dev.root.ref = list_new();
	dev.freeNodes = 0;

	module_register(&mod, "tmpfs", NULL, NULL);
	device_register(&dev.dev, &mod);
	device_register_bus(&dev.dev, FILESYSTEM, &dev.bus);
	dev.bus.dev = &dev.dev;
	return &dev.bus;
}













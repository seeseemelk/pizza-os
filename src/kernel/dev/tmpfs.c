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
#define MAX_FILENAME 16

/* A general node structure. */
typedef struct
{
	int id;
	char name[MAX_FILENAME];
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
	list_t* contents;
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
	dirnode_t root;
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
dirnode_t* tmpfs_find_subdir(dirnode_t* node, path_t* path)
{
	list_t* children = node->contents;
	size_t length = list_size(children);

	node_t* child;
	for (size_t i = 0; i < length; i++)
	{
		child = list_get(children, i);
		if (pcmp(child->name, path) == 0)
		{
			if (child->type == FDIR)
			{
				return (dirnode_t*) child;
			}
			else
			{
				kernel_panic("Node is not a directory");
				return NULL;
			}
		}
	}
	kernel_panic("Node not found");
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

	dirnode_t* node = &fs->root;
	while (path_next(&p))
	{
		node = tmpfs_find_subdir(node, &p);
	}
	return node->node.id;
}

/**
 * Finds a free unused inode in the device's
 * node list.
 */
int tmpfs_get_unused_inode(fs_t* fs)
{
	/* If there are free nodes, we'll find one and use it.
	 * If there aren't any free nodes, we'll add a new one.
	 */
	if (fs->freeNodes > 0)
	{
		size_t index = 0;
		size_t size = list_size(fs->nodes);
		for (size_t i = 0; i < size; i++)
		{
			if (list_get(fs->nodes, i) == NULL)
			{
				index = i;
				break;
			}
		}
		return index;
	}
	else
	{
		list_add(fs->nodes, NULL);
		return list_size(fs->nodes);
	}
}

/**
 * Stores a newly generated node into the device's
 * node list.
 */
void tmpfs_store_node(fs_t* fs, node_t* child, const char* name)
{
	/* Find a free spot in the list */
	int inode = tmpfs_get_unused_inode(fs);
	list_set(fs->nodes, inode, child);
	child->id = inode;
	strncpy(child->name, name, MAX_FILENAME);
}

void tmpfs_free_inode(device_t* dev, int inode)
{
	fs_t* fs = FS(dev);
	node_t* node = list_get(fs->nodes, inode);
	free(node);
	list_set(fs->nodes, inode, NULL);
}

int tmpfs_mkdir(device_t* dev, int parentInode, const char* name)
{
	fs_t* fs = FS(dev);
	dirnode_t* parent = list_get(fs->nodes, parentInode);
	dirnode_t* child = malloc(sizeof(dirnode_t));
	list_add(parent->contents, child);
	tmpfs_store_node(fs, (node_t*) child, name);
	return child->node.id;
}

filesystem_t* tmpfs_init()
{
	dev.bus.get_inode = tmpfs_get_inode;

	dev.root.contents = list_new();
	dev.freeNodes = 0;

	module_register(&mod, "tmpfs", NULL, NULL);
	device_register(&dev.dev, &mod);
	device_register_bus(&dev.dev, FILESYSTEM, &dev.bus);
	dev.bus.dev = &dev.dev;
	return &dev.bus;
}













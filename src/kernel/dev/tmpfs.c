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
#define NODE(n) ((node_t*) n)
#define DIR(n) ((dirnode_t*) n)
#define FILE(n) ((filenode_t*) n)
#define DEVN(n) ((devnode_t*) n)
#define CHUNK_SIZE 512

typedef struct node_t node_t;
typedef struct dirnode_t dirnode_t;
typedef struct filenode_t filenode_t;
typedef struct devnode_t devnode_t;

/* A general node structure. */
struct node_t
{
	int id;
	char name[MAX_FILENAME];
	dirnode_t* parent;
	file_t type;
	bool deleted;
};

/* Node structure for directories. */
struct dirnode_t
{
	node_t node;
	/* Contains a list to all the inodes that are
	 * in this directory.
	 */
	list_t* contents;
};

/* Node structure for files. */
struct filenode_t
{
	node_t node;
	/* Contains pointers to 512-byte buffers that
	 * hold the file data.
	 */
	list_t* contents;
	long long size;
};

/* Node structure for special files */
struct devnode_t
{
	node_t node;
	MAJOR major;
	MINOR minor;
};

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

/* Represents an opened directory. */
typedef struct
{
	dirnode_t* node;
	size_t pointer;
} opendir_t;

typedef struct
{
	filenode_t* node;
	mode_t mode;
	size_t pointer;
} openfile_t;

static filesystem_mounter_t fsmounter;
static module_t mod;
//static fs_t dev;

/**
 * Takes an inode and searches for a child entry in that
 * inode that has a name equal to path.
 */
node_t* tmpfs_find_child(dirnode_t* node, path_t* path)
{
	list_t* children = node->contents;
	size_t length = list_size(children);

	node_t* child;
	for (size_t i = 0; i < length; i++)
	{
		child = list_get(children, i);
		if (pcmp(child->name, path) == 0)
			return child;
	}
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

	node_t* node = NODE(&fs->root);
	/* TODO: Either path_next or path_store seems to have a bug. */
	while (path_next(&p))
	{
		char name[p.length];
		path_store(&p, name);
		//kernel_log("Entered path `%s`", name);
		if (node->type == FDIR)
		{
			node = tmpfs_find_child(DIR(node), &p);
			if (node == NULL)
				return 0;
		}
		else
			kernel_panic("Tried to go into file");
	}
	return node->id;
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
		return list_size(fs->nodes) - 1;
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
	child->id = inode + 1;
	child->deleted = false;
	strncpy(child->name, name, MAX_FILENAME);
}

/**
 * Gets a specific inode from a filesystem.
 */
node_t* tmpfs_get_node(fs_t* fs, int inode)
{
	return list_get(fs->nodes, inode - 1);
}

void tmpfs_free_inode(device_t* dev, int inode)
{
	fs_t* fs = FS(dev);
	node_t* node = NODE(tmpfs_get_node(fs, inode));
	/* We only free the node if it has been marked for deletion. */
	if (node->deleted)
	{
		if (node->type == FDIR)
		{
			dirnode_t* dir = DIR(node);
			list_free(dir->contents);
		}
		else if (node->type == FFILE)
		{
			filenode_t* file = FILE(node);
			size_t len = list_size(file->contents);
			for (size_t i = 0; i < len; i++)
			{
				free(list_get(file->contents, i));
			}
			list_free(file->contents);
		}
		list_set(fs->nodes, node->id - 1, NULL);
		free(node);
	}
}

int tmpfs_mkdir(device_t* dev, int parentInode, const char* name)
{
	fs_t* fs = FS(dev);

	/* Find the parent */
	dirnode_t* parent = DIR(tmpfs_get_node(fs, parentInode));

	/* Allocate and prepare the child */
	dirnode_t* child = malloc(sizeof(dirnode_t));
	child->contents = list_new();
	child->node.type = FDIR;
	child->node.parent = parent;

	/* Store the child. This will fill in several fields of the child too. */
	list_add(parent->contents, child);
	tmpfs_store_node(fs, (node_t*) child, name);
	return child->node.id;
}

int tmpfs_mkfile(device_t* dev, int parentInode, const char* name)
{
	fs_t* fs = FS(dev);

	/* Find the parent */
	dirnode_t* parent = DIR(tmpfs_get_node(fs, parentInode));

	/* Allocate and prepare the child */
	filenode_t* child = malloc(sizeof(filenode_t));
	child->contents = list_new();
	child->size = 0;
	child->node.type = FFILE;
	child->node.parent = parent;

	/* Store the child. This will fill in several fields of the child too. */
	list_add(parent->contents, child);
	tmpfs_store_node(fs, (node_t*) child, name);
	return child->node.id;
}

int tmpfs_mknode(device_t* dev, int parentInode, const char* name, file_t file, MAJOR major, MINOR minor)
{
	fs_t* fs = FS(dev);

	/* Find the parent */
	dirnode_t* parent = DIR(tmpfs_get_node(fs, parentInode));

	/* Allocate and prepare the child */
	devnode_t* child = malloc(sizeof(devnode_t));
	child->node.type = file;
	child->node.parent = parent;
	child->major = major;
	child->minor = minor;

	/* Store the child. This will fill in several fields of the child too. */
	list_add(parent->contents, child);
	tmpfs_store_node(fs, (node_t*) child, name);
	return child->node.id;
}

void tmpfs_rm(device_t* dev, int inode)
{
	/* Get the node */
	fs_t* fs = FS(dev);
	node_t* node = tmpfs_get_node(fs, inode);

	/* Mark the node for deletion */
	node->deleted = true;

	/* Remove the node from the global list of nodes */
	dirnode_t* parent = node->parent;
	size_t len = list_size(parent->contents);
	for (size_t i = 0; i < len; i++)
	{
		node_t* child = list_get(parent->contents, i);
		if (child == node)
		{
			list_set(parent->contents, i, NULL);
			break;
		}
	}
}

void tmpfs_stat(device_t* dev, int inode, stat_t* stat)
{
	/* Get the node */
	fs_t* fs = FS(dev);
	node_t* node = tmpfs_get_node(fs, inode);

	/* Stat the file */
	stat->type = node->type;
	memcpy(stat->name, node->name, MAX_FILENAME);

	if (stat->type == FDIR || stat->type == FFILE)
		stat->device = dev;
	else
	{
		devnode_t* devn = DEVN(node);
		stat->device = device_get_by_minor(devn->major, devn->minor);
	}
}

void* tmpfs_dir_open(device_t* dev, int inode)
{
	fs_t* fs = FS(dev);
	dirnode_t* node = (dirnode_t*) tmpfs_get_node(fs, inode);
	if (NODE(node)->type != FDIR)
		kernel_panic("Opened non-directory");
	opendir_t* od = malloc(sizeof(opendir_t));
	od->node = node;
	od->pointer = 0;
	return od;
}

void tmpfs_dir_close(device_t* dev, void* dirit)
{
	UNUSED(dev);
	free(dirit);
}

bool tmpfs_dir_next(device_t* dev, void* dirit, dirent_t* dirent)
{
	UNUSED(dev);
	opendir_t* od = (opendir_t*) dirit;

	while (od->pointer < list_size(od->node->contents))
	{
		node_t* child = list_get(od->node->contents, od->pointer);
		od->pointer++;
		if (child != NULL)
		{
			dirent->type = child->type;
			strncpy(dirent->name, child->name, MAX_FILENAME);
			return true;
		}
	}
	return false;
}

void* tmpfs_file_open(device_t* dev, int inode, mode_t mode)
{
	fs_t* fs = FS(dev);
	openfile_t* of = malloc(sizeof(openfile_t));
	of->node = (filenode_t*) tmpfs_get_node(fs, inode);
	of->pointer = 0;
	of->mode = mode;
	return of;
}

void tmpfs_file_close(device_t* dev, void* data)
{
	UNUSED(dev);
	free(data);
}

char* tmpfs_file_get_chunk(openfile_t* of, size_t* chunk_size)
{
	size_t chunk_index = of->pointer / CHUNK_SIZE;
	size_t chunk_offset = of->pointer % CHUNK_SIZE;
	*chunk_size = CHUNK_SIZE - chunk_offset;

	//if (chunk_offset == 0)
	if (chunk_index >= list_size(of->node->contents))
	{
		char* filebuf = malloc(CHUNK_SIZE);
		list_add(of->node->contents, filebuf);
		return filebuf;
	}
	else
	{
		char* filebuf = list_get(of->node->contents, chunk_index);
		return filebuf + chunk_offset;
	}
}

size_t tmpfs_file_write(device_t* dev, void* data, const char* buf, size_t len)
{
	UNUSED(dev);
	openfile_t* of = data;
	if ((of->mode & O_WRITE) == 0)
		kernel_panic("Tried to write to non-write descriptor");

	size_t left = len;
	size_t chunk_size;
	size_t amount;

	while (left > 0)
	{
		char* chunk_buf = tmpfs_file_get_chunk(of, &chunk_size);
		amount = (chunk_size < left) ? chunk_size : left;
		memcpy(chunk_buf, buf, amount);
		left -= amount;
		of->pointer += amount;
		buf += amount;
	}

	if (of->pointer > of->node->size)
		of->node->size = of->pointer;
	return len - left;
}

size_t tmpfs_file_read(device_t* dev, void* data, char* buf, size_t len)
{
	UNUSED(dev);
	openfile_t* of = data;
	if ((of->mode & O_READ) == 0)
		kernel_panic("Tried to read from non-read descriptor");

	if (len + of->pointer > of->node->size)
		len = of->node->size - of->pointer;

	size_t left = len;
	while (left > 0)
	{
		size_t chunk_size;
		char* chunk_buf = tmpfs_file_get_chunk(of, &chunk_size);
		size_t amount = (chunk_size < left) ? chunk_size : left;
		memcpy(buf, chunk_buf, amount);
		left -= amount;
		of->pointer += amount;
		buf += amount;
	}
	return len - left;
}

filesystem_t* tmpfs_mount(module_t* mod, const char* path, int argc, const char** argv)
{
	/*UNUSED(mod);
	UNUSED(path);
	UNUSED(argc);
	UNUSED(argv);
	kernel_panic("Operation not supported");*/

	UNUSED(path);
	UNUSED(argc);
	UNUSED(argv);

	fs_t* dev = malloc(sizeof(fs_t));
	dev->bus.dev = &dev->dev;
	dev->bus.get_inode = tmpfs_get_inode;
	dev->bus.free_inode = tmpfs_free_inode;
	dev->bus.mkdir = tmpfs_mkdir;
	dev->bus.mkfile = tmpfs_mkfile;
	dev->bus.mknode = tmpfs_mknode;
	dev->bus.rm = tmpfs_rm;
	dev->bus.stat = tmpfs_stat;
	dev->bus.dir_open = tmpfs_dir_open;
	dev->bus.dir_close = tmpfs_dir_close;
	dev->bus.dir_next = tmpfs_dir_next;
	dev->bus.file_open = tmpfs_file_open;
	dev->bus.file_close = tmpfs_file_close;
	dev->bus.file_write = tmpfs_file_write;
	dev->bus.file_read = tmpfs_file_read;

	dev->root.node.id = 1;
	dev->root.node.name[0] = '\0';
	dev->root.node.type = FDIR;
	dev->root.contents = list_new();
	dev->freeNodes = 0;
	dev->nodes = list_new();
	list_add(dev->nodes, &dev->root);

	device_register(&dev->dev, mod);
	device_register_bus(&dev->dev, FILESYSTEM, &dev->bus);
	return &dev->bus;
}

void tmpfs_init()
{
	fsmounter.mod = &mod;
	fsmounter.name = "tmpfs";
	fsmounter.mount = tmpfs_mount;

	module_register(&mod, "tmpfs", NULL, NULL);
	module_register_bus(&mod, FILESYSTEM, &fsmounter);
}













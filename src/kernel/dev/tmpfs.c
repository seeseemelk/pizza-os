#include "tmpfs.h"
#include "vfs.h"
#include "devices.h"
#include "cdefs.h"
#include "config.h"
#include "kernel.h"
#include "collections/iterator.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

module_t* tmpfs;

#define MAX_NODES 256
#define MAX_CHILDREN 16

typedef struct node_t
{
	char name[256];
	file_t type;
	unsigned int size;
	void* content;
} node_t;

typedef struct
{
	unsigned int next_id;
	node_t nodes[MAX_NODES];
} dev_info_t;

typedef struct
{
	node_t* inode;
	unsigned int index;
} dird_t;

/*
 * READ, WRITE, FLUSH,
 * MOUNT, STAT,
 * DIRIT_NEW, DIRIT_NEXT
 */

node_t* tmpfs_create_dir(dev_info_t* dev_info, node_t* parent, const char* name)
{
	unsigned int id = dev_info->next_id++;
	node_t* dir = dev_info->nodes + id;
	dir->type = EDIRECTORY;
	dir->content = calloc(sizeof(unsigned int), MAX_CHILDREN);
	dir->size = 0;
	strncpy(dir->name, name, 256);
	if (parent != NULL)
		((unsigned int*) parent->content)[parent->size++] = id;
	return dir;
}

int tmpfs_mod_req(mod_req_t* request)
{
	request_type type = request->type;
	if (type == MOUNT) {
		//device_t* device = device_register(tmpfs);
		dev_info_t* dev_info = malloc(sizeof(dev_info_t));
		node_t* root_dir = tmpfs_create_dir(dev_info, NULL, "");
		device->data = (void*) dev_info;
		tmpfs_create_dir(dev_info, root_dir, "dir");
		return (int) device;
	} else {
		kernel_panic("Unknown request type on TMPFS module");
		return 0;
	}
}

int tmpfs_dev_req(dev_req_t* request)
{
	dev_info_t* dev = (dev_info_t*) request->device->data;

	request_type type = request->type;
	if (type == DIROPEN) {
		dird_t* dird = malloc(sizeof(dird_t));
		dird->inode = dev->nodes + request->arg1;
		dird->index = 0;
		return (int) dird;
	} else if (type == DIRNEXT) {
		dird_t* dird = (dird_t*) request->arg1;
		if (dird->index < dird->inode->size)
		{
			dirent_t* dirent = (dirent_t*) request->arg2;
			unsigned int node_id = ((unsigned int*) dird->inode->content)[dird->index];
			node_t* node = dev->nodes + node_id;
			dird->index++;
			strncpy(dirent->name, node->name, 256);
			return 1;
		}
		else
			return 0;
	} else if (type == DIRCLOSE) {
		dird_t* dird = (dird_t*) request->arg1;
		free(dird);
		return 0;
	} else {
		kernel_panic("Unknown request type on TMPFS module");
		return 0;
	}
}

void tmpfs_init()
{
	tmpfs = module_register("tmpfs", FILESYSTEM, &tmpfs_mod_req, &tmpfs_dev_req);
	kprintf("tmpfs loaded as %d\n", tmpfs->major);
}













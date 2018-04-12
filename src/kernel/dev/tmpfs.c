#include "tmpfs.h"
#include "vfs.h"
#include "devices.h"
#include "cdefs.h"
#include "config.h"
#include "kernel.h"
#include "collections/iterator.h"
#include <stdio.h>
#include <stdlib.h>

module_t* tmpfs;

#define MAX_NODES 256
#define MAX_CHILDREN 16

typedef struct
{
	const char* name;
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
} it_data_t;

/*
 * READ, WRITE, FLUSH,
 * MOUNT, STAT,
 * DIRIT_NEW, DIRIT_NEXT
 */

void tmpfs_create_dir(dev_info_t* dev_info, unsigned int id)
{
	node_t* dir = dev_info->nodes + id;
	dir->type = DIRECTORY;
	dir->content = calloc(sizeof(unsigned int), MAX_CHILDREN);
	dir->size = 0;
}

int tmpfs_mod_req(mod_req_t* request)
{
	request_type type = request->type;
	if (type == MOUNT) {
		device_t* device = device_register(tmpfs);
		dev_info_t* dev_info = malloc(sizeof(dev_info_t));
		tmpfs_create_dir(dev_info, 0);
		dev_info->next_id = 1;
		device->data = (void*) dev_info;
		return (int) device;
	} else {
		kernel_panic("Unknown request type on TMPFS module");
		return 0;
	}
}

bool tmpfs_it_has_next(it_t* it)
{
	it_data_t* data = (it_data_t*) it->data;
	return data->index < data->inode->size;
}

void* tmpfs_it_next(it_t* it)
{
	it_data_t* data = (it_data_t*) it->data;
	return data->inode->content + (data->index++);
}

int tmpfs_dev_req(dev_req_t* request)
{
	dev_info_t* dev = (dev_info_t*) request->device->data;

	request_type type = request->type;
	if (type == DIRIT_CREATE) {
		unsigned int inode = request->arg1;
		it_t* it = it_new(&tmpfs_it_has_next, &tmpfs_it_next);
		it_data_t* data = malloc(sizeof(it_data_t));
		it->data = (void*) data;
		data->index = 0;
		data->inode = dev->nodes + inode;
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













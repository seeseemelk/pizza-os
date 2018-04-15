#include "vfs.h"
#include "cdefs.h"
#include "devices.h"
#include "collections/list.h"
#include "kernel.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

mount_t mounts[16];
size_t num_mounts = 0;

void vfs_save_devi(device_t* src, devi_t* dest)
{
	dest->dev_major = src->module->major;
	dest->dev_minor = src->minor;
}

device_t* vfs_getdev(devi_t* dev)
{
	return device_get_by_minor(dev->dev_major, dev->dev_minor);
}

device_t* vfs_finddev(const char* path)
{
	for (int i = num_mounts-1; i >= 0; i--)
	{
		if (strncmp(path, mounts[i].path, 256) == 0)
		{
			return mounts[i].dev;
		}
	}
	kernel_panic("Mount point not found");
	return NULL;
}

bool vfs_is_sibling(const char* parent, const char* sibling)
{
	size_t parent_len = strlen(parent);
	size_t sibling_len = strlen(sibling);
	if (sibling_len >= parent_len)
		return strncmp(sibling, parent, parent_len) == 0;
	else
		return false;
}

void vfs_diropen(DIR* dir, const char* path)
{
	device_t* dev = vfs_finddev(path);
	vfs_save_devi(dev, &dir->dev);
	dir->inode = 0;
	dir->dev_it = (void*) device_invoke1(dev, DIROPEN, dir->inode);
}

void vfs_dirclose(DIR* dir)
{
	device_t* dev = vfs_getdev(&dir->dev);
	device_invoke1(dev, DIRCLOSE, (int) dir->dev_it);
}

int vfs_dirnext(DIR* dir, dirent_t* dirent)
{
	device_t* dev = vfs_getdev(&dir->dev);
	return device_invoke2(dev, DIRNEXT, (int) dir->dev_it, (int) dirent);
}

void tvfs_list(const char* path)
{
	kprintf("-> %s\n", path);
	DIR dir;
	dirent_t dirent;
	vfs_diropen(&dir, path);
	while (vfs_dirnext(&dir, &dirent) != 0)
	{
		kprintf("%s%s\n", path, dirent.name);
		if (dirent.type == EDIRECTORY)
		{
			size_t path_len = strlen(path);
			size_t dir_len = strlen(dirent.name);
			char* subpath = malloc(path_len + dir_len + 2);
			strcpy(subpath, path);
			strcpy(subpath + path_len, dirent.name);
			subpath[path_len + dir_len + 1] = '/';
			subpath[path_len + dir_len + 2] = 0;
			tvfs_list(subpath);
			free(subpath);
		}
	}
	vfs_dirclose(&dir);
}

void tvfs_test()
{
	kprintf("Listing '/'\n");
	tvfs_list("/");
	kprintf("Done\n");
	/*DIR dir;
	vfs_diropen(&dir, "/");
	dirent_t dirent;
	kprintf("Listing\n");
	tv*/
}

void vfs_init()
{
	module_t* tmpfs = module_get("tmpfs");
	if (tmpfs == NULL)
		kernel_panic("tmpfs module not loaded");
	mounts[0].dev = (device_t*) module_invoke(tmpfs, MOUNT);
	//vfs_save_devi(dev, &mounts[0].dev);
	mounts[0].path = "/";
	num_mounts++;

	tvfs_test();
}

//stat_t root;

/*void vfs_stat(unsigned int inode, stat_t* dest_stat)
{
	stat_t* stat = list_get(inodes, inode);
	dest_stat->inode = stat->inode;
	dest_stat->filename = stat->filename;
	dest_stat->size = stat->size;
	dest_stat->type = stat->type;
}*/

/*
void vfs_mount(unsigned int inode, module_t* module)
{
	stat_t* stat = list_get(inodes, inode);
	device_t* dev = device_invoke1(module, MOUNT, stat->inode);
	stat->dev = stat;
}

void vfs_dir_begin(stat_t* inode, dir_it_t* it)
{
	it->dev = inode;
	it->iterator = device_invoke1(it->dev, DIRIT_CREATE, inode->inode);
}

void vfs_dir_next(dir_it_t* it, stat_t* stat)
{
	unsigned int inode = it_next(it->iterator);
}

void vfs_dir_end(dir_it_t* it)
{
	free(it->iterator);
}

void tvfs_print(unsigned int inode)
{
	stat_t stat = root;
	//vfs_stat(inode, &stat);
	kprintf("%c '%s' %d %d\n", stat.type==FILE?'F':'D', stat.filename, stat.inode, stat.size);
}

void tvfs_test()
{
	tvfs_print(root);
}

void vfs_init()
{
	module_t* tmpfs = module_get("tmpfs");
	if (tmpfs == NULL)
		kernel_panic("tmpfs module not loaded");

	root.filename = "";
	root.type = DIRECTORY;
	root.dev = (device_t*) module_invoke(tmpfs, MOUNT);

	tvfs_test();
}

/ *
void vfs_fs_stat(device_t* dev, stat_t* stat, unsigned int inode)
{
	device_invoke2(dev, STAT, (int)inode, (int)stat);
}

int vfs_stat(const char* path, stat_t* stat)
{
	UNUSED(path);
	UNUSED(stat);
	return 0;
}

typedef struct
{
	stat_t* parent;
	it_t* dev_it;
} it_dir_t;

bool vfs_dir_iter_has_next(it_t* it)
{
	it_dir_t* data = (it_dir_t*) it->data;
	return it_has_next(data->dev_it);
}

void* vfs_dir_iter_next(it_t* it)
{
	it_dir_t* data = (it_dir_t*) it->data;
	unsigned int inode = it_next(data->dev_it);

	return it_has_next(data->dev_it);
}

void vfs_dir_iter_free(it_t* it)
{
	it_dir_t* data = (it_dir_t*) it->data;
	it_free(data->dev_it);
}

it_t* vfs_dir_iter(stat_t* parent)
{
	it_t* it = it_new(&vfs_dir_iter_has_next, &vfs_dir_iter_next);
	it_dir_t* data = malloc(sizeof(it_dir_t));
	data->parent = parent;
	data->dev_it = (it_t*) device_invoke1(parent->dev, DIRIT_CREATE, parent->inode);
	it->data = data;
	it->free = &vfs_dir_iter_free;
	return it;
}

void vfs_test()
{
	dir_it it;
	vfs_create_dir_it(&it);
}

void vfs_init()
{
	device_t* tmpfs;
	root.filename = "";
	root.type = DIRECTORY;
	root.dev = (device_t*) device_invoke(tmpfs, MOUNT);

	vfs_test();
}*/

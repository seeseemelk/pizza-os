#include "vfs.h"
#include "cdefs.h"
#include "devices.h"
#include "collections/list.h"
#include "kernel.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* Contains a list of all mountpoints */
list_t* mountpoints;

/* Data structure stored in mountpoints list */
typedef struct
{
	char path[256];
	filesystem_t* fs;
} mountpoint_t;

/* Contains a list of all directory descriptors */
typedef struct
{
	filesystem_t* fs;
	size_t inode;
	void* data;
} open_dir_t;
list_t* open_dirs;

/* Contains a list of all open file descriptors */
typedef struct
{
	filesystem_t* fs;
	file_t type;
	union {
		/* The file is just a common file descriptor */
		size_t inode;

		/* The file is an opened special file */
		fileop_t* fop;
	};
	void* data;
} open_file_t;
list_t* open_files;

void vfs_init()
{
	mountpoints = list_new();
	open_dirs = list_new();
	open_files = list_new();

	/* This prevents the VFS from ever using descriptor 0. */
	list_add(open_dirs, (void*) 0xFFFFFFFF);
	list_add(open_files, (void*) 0xFFFFFFFF);
}

filesystem_mounter_t* vfs_get_mounter(const char* name)
{
	bus_it it;
	filesystem_mounter_t* fs;
	module_it_begin(&it, FILESYSTEM);
	while ((fs = module_it_next(&it)) != NULL)
	{
		if (strcmp(name, fs->name) == 0)
			return fs;
	}
	kernel_panic("No such filesystem mounter");
	return NULL;
}

void vfs_mount_direct(const char* path, filesystem_t* fs)
{
	mountpoint_t* mp = malloc(sizeof(mp));
	mp->fs = fs;
	strcpy(mp->path, path);
	list_add(mountpoints, mp);
	kernel_log("Mounted %s at %s", fs->dev->module->name, path);
}

void vfs_mount(const char* path, const char* fs, const char* block, int argc, const char** argv)
{
	filesystem_mounter_t* mounter = vfs_get_mounter(fs);
	filesystem_t* mounted = mounter->mount(mounter->mod, block, argc, argv);
	vfs_mount_direct(path, mounted);
}

mountpoint_t* vfs_find_mountpoint(const char* path)
{
	size_t path_length = strlen(path);
	size_t mp_size = list_size(mountpoints);
	/* TODO Check if the loop condition is correct. */
	for (size_t i = mp_size-1; i < mp_size; i--)
	{
		mountpoint_t* mp = list_get(mountpoints, i);
		size_t mp_length = strlen(mp->path);
		if (path_length >= mp_length && strncmp(path, mp->path, mp_length) == 0)
		{
			return mp;
		}
	}
	kernel_panic("No mountpoint found");
	return NULL;
}

const char* vfs_get_sub_path(mountpoint_t* mp, const char* path)
{
	size_t fs_len = strlen(mp->path);
	return path + fs_len - 1;
}

/**
 * Checks if the inode can be freed.
 * If it can be freed it will call `free_inode` to the filesystem.
 */
void vfs_update_freed(filesystem_t* fs, size_t inode)
{
	size_t len;
	/* Check if a directory entry is using the inode. */
	len = list_size(open_dirs);
	for (size_t i = 1; i < len; i++)
	{
		open_dir_t* od = list_get(open_dirs, i);
		if (od != NULL && od->inode == inode && od->fs == fs)
			return; /* We return if the inode is still being used */
	}

	/* The same but for files */
	len = list_size(open_files);
	for (size_t i = 1; i < len; i++)
	{
		open_file_t* of = list_get(open_files, i);
		if (of != NULL && of->inode == inode && of->fs == fs)
			return; /* We return if the inode is still being used */
	}

	/* Free the inode */
	fs->free_inode(fs->dev, inode);
}

size_t vfs_find_free_descriptor(list_t* list)
{
	size_t len = list_size(list);
	for (size_t i = 1; i < len; i++)
	{
		if (list_get(list, i) == NULL)
			return i;
	}

	/* None were found, so we make one */
	list_add(list, NULL);
	return list_size(list) - 1;
}

bool vfs_rm(const char* path)
{
	/* Find the mountpoint and subpath */
	mountpoint_t* mp = vfs_find_mountpoint(path);
	filesystem_t* fs = mp->fs;
	const char* subpath = vfs_get_sub_path(mp, path);

	/* Get the inode */
	int inode = fs->get_inode(fs->dev, subpath);

	/* Check if it is a directory */
	stat_t stat;
	vfs_stat(path, &stat);
	if (stat.type == FDIR)
	{
		/* Check if it's empty */
		DIR dir = vfs_open_dir(path);
		dirent_t dirent;
		bool result = vfs_next_dir(dir, &dirent);
		vfs_close_dir(dir);
		if (result == true)
		{
			/* It's not empty */
			vfs_update_freed(fs, inode);
			return false;
		}
	}

	/* Delete it */
	fs->rm(fs->dev, inode);
	vfs_update_freed(fs, inode);
	return true;
}

void vfs_mkdir(const char* path)
{
	mountpoint_t* mp = vfs_find_mountpoint(path);
	filesystem_t* fs = mp->fs;
	const char* subpath = vfs_get_sub_path(mp, path);
	char* parent = path_parent(subpath);
	char* filename = path_filename(subpath);
	int parent_inode = fs->get_inode(fs->dev, parent);
	if (parent_inode == 0)
		kernel_panic("Parent directory of %s does not exist", path);
	int inode = fs->mkdir(fs->dev, parent_inode, filename);
	vfs_update_freed(fs, parent_inode);
	vfs_update_freed(fs, inode);
	free(parent);
}

void vfs_mkspecial(const char* path, device_t* dev, file_t type)
{
	mountpoint_t* mp = vfs_find_mountpoint(path);
	filesystem_t* fs = mp->fs;
	const char* subpath = vfs_get_sub_path(mp, path);
	char* parent = path_parent(subpath);
	char* filename = path_filename(subpath);
	int parent_inode = fs->get_inode(fs->dev, parent);
	if (parent_inode == 0)
		kernel_panic("Parent directory of %s does not exist", path);
	int inode = fs->mknode(fs->dev, parent_inode, filename, type, dev->module->major, dev->minor);
	vfs_update_freed(fs, parent_inode);
	vfs_update_freed(fs, inode);
	free(parent);
}

void vfs_mkblock(const char* path, device_t* dev)
{
	vfs_mkspecial(path, dev, FBLOCK);
}

void vfs_mkchar(const char* path, device_t* dev)
{
	vfs_mkspecial(path, dev, FCHAR);
}

/* Functions that operate on opened directories */

DIR vfs_open_dir(const char* path)
{
	/* Find the mounted filesystem and the relative subpath */
	mountpoint_t* mp = vfs_find_mountpoint(path);
	filesystem_t* fs = mp->fs;
	const char* subpath = vfs_get_sub_path(mp, path);

	/* Get the inode */
	int inode = fs->get_inode(fs->dev, subpath);
	if (inode == 0)
		kernel_panic("Directory does not exist");

	/* Open the directory */
	void* data = fs->dir_open(fs->dev, inode);
	open_dir_t* od = malloc(sizeof(open_dir_t));
	od->fs = fs;
	od->inode = inode;
	od->data = data;

	/* Store the descriptor */
	/*list_add(open_dirs, od);
	DIR dir = list_size(open_dirs) - 1;*/
	DIR dir = vfs_find_free_descriptor(open_dirs);
	list_set(open_dirs, dir, od);
	return dir;
}

void vfs_close_dir(DIR dir)
{
	open_dir_t* od = list_get(open_dirs, dir);
	filesystem_t* fs = od->fs;
	fs->dir_close(fs->dev, od->data);
	list_set(open_dirs, dir, NULL);
	vfs_update_freed(fs, od->inode);
	free(od);
}

bool vfs_next_dir(DIR dir, dirent_t* dirent)
{
	open_dir_t* od = list_get(open_dirs, dir);
	filesystem_t* fs = od->fs;
	return fs->dir_next(fs->dev, od->data, dirent);
}

/* Functions that operate on opened files */

FILE vfs_open_file(const char* path, mode_t mode)
{
	mountpoint_t* mp = vfs_find_mountpoint(path);
	filesystem_t* fs = mp->fs;
	const char* subpath = vfs_get_sub_path(mp, path);

	/* Needs a way to check if the inode exists.
	 * If it does it should be opened,
	 * if it doesn't it may need to be created.
	 */

	int inode = fs->get_inode(fs->dev, subpath);
	if (inode == 0) /* The node does not exist. Create it (if allowed to). */
	{
		if (mode & O_CREATE) /* We're allowed to create a new file. */
		{
			char* parent = path_parent(subpath);
			char* filename = path_filename(subpath);
			int parent_inode = fs->get_inode(fs->dev, parent);
			inode = fs->mkfile(fs->dev, parent_inode, filename);
			vfs_update_freed(fs, parent_inode);
			vfs_update_freed(fs, inode);
			free(parent);
			free(filename);
		}
		else
		{
			kernel_log("Opened file that does not exist, not creating");
			return 0;
		}
	}

	/* Create descriptors */
	FILE desc = vfs_find_free_descriptor(open_files);
	open_file_t* of = malloc(sizeof(open_file_t));
	of->fs = fs;
	list_set(open_files, desc, of);

	/* Test the file type */
	stat_t stat;
	fs->stat(fs->dev, inode, &stat);
	of->type = stat.type;
	if (stat.type == FFILE)
	{
		/* File is just a normal file. Open it. */
		void* data = fs->file_open(fs->dev, inode, mode);

		of->inode = inode;
		of->data = data;

		return desc;
	}
	else if (stat.type == FCHAR || stat.type == FBLOCK)
	{
		/* File is a special file. So handle it specially. */
		fileop_t* fop = device_get_bus(stat.device, FILEOP);
		of->fop = fop;
		of->data = fop->open(fop->dev);
		return desc;
	}
	else
	{
		kernel_panic("Bad fs: stat.type was not filled in correctly");
		return 0;
	}

	//open_files_fs
}

void vfs_close_file(FILE file)
{
	open_file_t* of = list_get(open_files, file);
	list_set(open_files, file, NULL);
	if (of->type == FFILE)
	{
		/* Normal file */
		filesystem_t* fs = of->fs;
		fs->file_close(fs->dev, of->data);
		vfs_update_freed(fs, of->inode);
	}
	else
	{
		/* Special file */
		fileop_t* fop = of->fop;
		fop->close(fop->dev, of->data);
	}
	/* Cleanup descriptor */
	free(of);
}

size_t vfs_write_file(FILE file, const char* buf, size_t len)
{
	if (file == 0)
		kernel_panic("Tried to write to unopened file");

	open_file_t* of = list_get(open_files, file);
	if (of->type == FFILE)
	{
		filesystem_t* fs = of->fs;
		return fs->file_write(fs->dev, of->data, buf, len);
	}
	else
	{
		fileop_t* fop = of->fop;
		return fop->write(fop->dev, of->data, buf, len);
	}
}

size_t vfs_read_file(FILE file, char* buf, size_t len)
{
	if (file == 0)
			kernel_panic("Tried to read from unopened file");

	open_file_t* of = list_get(open_files, file);
	if (of->type == FFILE)
	{
		filesystem_t* fs = of->fs;
		return fs->file_read(fs->dev, of->data, buf, len);
	}
	else
	{
		fileop_t* fop = of->fop;
		return fop->read(fop->dev, of->data, buf, len);
	}
}

/* Functions that operate on closed files */

bool vfs_stat(const char* path, stat_t* stat)
{
	/* Find the mountpoint and subpath */
	mountpoint_t* mp = vfs_find_mountpoint(path);
	filesystem_t* fs = mp->fs;
	const char* subpath = vfs_get_sub_path(mp, path);

	/* Get the inode */
	int inode = fs->get_inode(fs->dev, subpath);
	if (inode == 0) /* Inode not found */
		return false;

	/* Stat the inode */
	fs->stat(fs->dev, inode, stat);
	stat->device = fs->dev;

	/* Cleanup */
	vfs_update_freed(fs, inode);
	return true;
}















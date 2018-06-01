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
list_t* dir_desc_fs;
list_t* dir_desc_it;

/* Contains a list of all open file descriptors */
typedef struct
{
	filesystem_t* fs;
	void* data;
} open_file_t;
list_t* open_files;

void vfs_init()
{
	mountpoints = list_new();
	dir_desc_fs = list_new();
	dir_desc_it = list_new();
	open_files = list_new();
}

void vfs_mount(const char* path, filesystem_t* fs)
{
	mountpoint_t* mp = malloc(sizeof(mp));
	mp->fs = fs;
	strcpy(mp->path, path);
	list_add(mountpoints, mp);
	kernel_log("Mounted %s at %s", fs->dev->module->name, path);
}

mountpoint_t* vfs_find_mountpoint(const char* path)
{
	size_t path_length = strlen(path);
	size_t mp_size = list_size(mountpoints);
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

void vfs_mkdir(const char* path)
{
	mountpoint_t* mp = vfs_find_mountpoint(path);
	filesystem_t* fs = mp->fs;
	const char* subpath = vfs_get_sub_path(mp, path);
	//fs->mkdir(fs->dev, subpath);
	char* parent = path_parent(subpath);
	char* filename = path_filename(subpath);
	int parent_inode = fs->get_inode(fs->dev, parent);
	fs->mkdir(fs->dev, parent_inode, filename);
	free(parent);
}

/* Functions that operate on opened directories */

DIR vfs_open_dir(const char* path)
{
	mountpoint_t* mp = vfs_find_mountpoint(path);
	filesystem_t* fs = mp->fs;
	const char* subpath = vfs_get_sub_path(mp, path);
	int inode = fs->get_inode(fs->dev, subpath);
	void* dirit = fs->dir_open(fs->dev, inode);
	list_add(dir_desc_fs, fs);
	list_add(dir_desc_it, dirit);
	DIR dir = list_size(dir_desc_fs) - 1;
	return dir;
}

void vfs_close_dir(DIR dir)
{
	filesystem_t* fs = list_get(dir_desc_fs, dir);
	void* dirit = list_get(dir_desc_it, dir);
	fs->dir_close(fs->dev, dirit);
	list_set(dir_desc_fs, dir, NULL);
	list_set(dir_desc_it, dir, NULL);
}

bool vfs_next_dir(DIR dir, dirent_t* dirent)
{
	filesystem_t* fs = list_get(dir_desc_fs, dir);
	void* dirit = list_get(dir_desc_it, dir);
	return fs->dir_next(fs->dev, dirit, dirent);
}

/* Functions that operate on opened files */

size_t vfs_find_free_descriptor(list_t* list)
{
	size_t len = list_size(list);
	for (size_t i = 0; i < len; i++)
	{
		if (list_get(list, i) == NULL)
			return i;
	}

	/* None were found, so we make one */
	list_add(list, NULL);
	return list_size(list) - 1;
}

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
	if (inode == 0) /* The node does not exist. Create it. */
	{
		char* parent = path_parent(subpath);
		char* filename = path_filename(subpath);
		inode = fs->get_inode(fs->dev, parent);
		fs->mkfile(fs->dev, inode, filename);
		free(parent);
		free(filename);
		inode = fs->get_inode(fs->dev, subpath);
	}

	/* Open the file */
	void* data = fs->file_open(fs->dev, inode, mode);

	/* Save the descriptor */
	FILE desc = vfs_find_free_descriptor(open_files);
	open_file_t* of = malloc(sizeof(open_file_t));
	of->fs = fs;
	of->data = data;
	list_set(open_files, desc, of);

	return desc;
	//open_files_fs
}

void vfs_close_file(FILE file)
{
	open_file_t* of = list_get(open_files, file);
	filesystem_t* fs = of->fs;
	fs->file_close(fs->dev, of->data);
	list_set(open_files, file, NULL);
}

size_t vfs_write_file(FILE file, const char* buf, size_t len)
{
	open_file_t* of = list_get(open_files, file);
	filesystem_t* fs = of->fs;
	return fs->file_write(fs->dev, of->data, buf, len);
}

size_t vfs_read_file(FILE file, char* buf, size_t len)
{
	open_file_t* of = list_get(open_files, file);
	filesystem_t* fs = of->fs;
	return fs->file_read(fs->dev, of->data, buf, len);
}

/* Functions that operate on closed files */

void vfs_stat(const char* path)
{
	mountpoint_t* mp = vfs_find_mountpoint(path);
	filesystem_t* fs = mp->fs;
	const char* subpath = vfs_get_sub_path(mp, path);
	int inode = fs->get_inode(fs->dev, subpath);
	fs->stat(fs->dev, inode);
}


#include "vfs.h"

stat_t root;

int vfs_stat(const char* path, stat_t* stat)
{

}

void vfs_init()
{
	root.filename = "";
	root.type = DIRECTORY;
}

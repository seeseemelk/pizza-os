#include "tmpfs.h"
#include "devices.h"

module* mod;

int tmpfs_request(module_request* request)
{

}

void tmpfs_init()
{
	mod = module_register("tmpfs", FILESYSTEM, &tmpfs_request);
}

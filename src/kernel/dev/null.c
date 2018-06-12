#include "null.h"
#include "cdefs.h"
#include "devices.h"
#include "bus/filesystem.h"
#include "vfs.h"

static module_t mod;
static device_t dev;

void* null_open(device_t* dev)
{
	UNUSED(dev);
	return NULL;
}

void null_close(device_t* dev, void* fd)
{
	UNUSED(dev);
	UNUSED(fd);
}

size_t null_write(device_t* dev, void* fd, const char* buf, size_t buf_len)
{
	UNUSED(dev);
	UNUSED(fd);
	UNUSED(buf);
	UNUSED(buf_len);
	return 0;
}

size_t null_read(device_t* dev, void* fd, char* buf, size_t buf_len)
{
	UNUSED(dev);
	UNUSED(fd);
	UNUSED(buf);
	UNUSED(buf_len);
	return 0;
}

static fileop_t fopt = {
	.dev = &dev,
	.open = null_open,
	.close = null_close,
	.write = null_write,
	.read = null_read
};

void null_init()
{
	module_register(&mod, "null", NULL, NULL);
	device_register(&dev, &mod);
	device_register_bus(&dev, FILEOP, &fopt);
	vfs_mkchar("/dev/null", &dev);
}

#include "null.h"
#include "cdefs.h"
#include "devices.h"
#include "bus/filesystem.h"
#include "vfs.h"

static module_t mod;
static device_t dev;

void* zero_open(device_t* dev)
{
	UNUSED(dev);
	return NULL;
}

void zero_close(device_t* dev, void* fd)
{
	UNUSED(dev);
	UNUSED(fd);
}

size_t zero_write(device_t* dev, void* fd, const char* buf, size_t buf_len)
{
	UNUSED(dev);
	UNUSED(fd);
	UNUSED(buf);
	return buf_len;
}

size_t zero_read(device_t* dev, void* fd, char* buf, size_t buf_len)
{
	UNUSED(dev);
	UNUSED(fd);
	for (size_t i = 0; i < buf_len; i++)
		buf[i] = 0;
	return buf_len;
}

static fileop_t fopt = {
	.dev = &dev,
	.open = zero_open,
	.close = zero_close,
	.write = zero_write,
	.read = zero_read
};

void zero_init()
{
	module_register(&mod, "zero", NULL, NULL);
	device_register(&dev, &mod);
	device_register_bus(&dev, FILEOP, &fopt);
	vfs_mkchar("/dev/zero", &dev);
}

#include "syscall.hpp"

#include "debug.hpp"
#include "vfs.hpp"

u32 Syscall::VFS::is_open(u32 fd, u32 _1, u32 _2)
{
	UNUSED(_1);
	UNUSED(_2);

	return ::VFS::is_open(fd);
}

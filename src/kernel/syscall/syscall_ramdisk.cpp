#include "syscall.hpp"

#include "debug.hpp"

using namespace Syscall;

u32 Ramdisk::open(u32 ebx, u32 ecx, u32 edx)
{
	UNUSED(ebx);
	UNUSED(ecx);
	UNUSED(edx);
	return 0;
}

u32 Ramdisk::close(u32 ebx, u32 ecx, u32 edx)
{
	UNUSED(ebx);
	UNUSED(ecx);
	UNUSED(edx);
	return 0;
}

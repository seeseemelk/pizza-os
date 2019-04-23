#include <pizzaos/syscall.h>
#include <stdbool.h>

void syscall_exit(void)
{
	syscall(0);
}

bool syscall_vfs_is_open(u32 fd)
{
	return syscall(1, fd);
}

void syscall_debug_log3(const char* fmt, u32 a, u32 b)
{
	syscall(2, fmt, a, b);
}


void syscall_debug_log2(const char* fmt, u32 a)
{
	syscall(2, fmt, a);
}


void syscall_debug_log(const char* fmt)
{
	syscall(2, fmt);
}

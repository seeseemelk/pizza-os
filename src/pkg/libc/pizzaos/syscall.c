#include "pizzaos/syscall.h"
#include <stdbool.h>
#include <syscall_nums.h>

void syscall_exit(void)
{
	syscall(SYSCALL_EXIT);
}

bool syscall_vfs_is_open(u32 fd)
{
	return syscall(SYSCALL_VFS_ISOPEN, fd);
}

void syscall_debug_log3(const char* fmt, u32 a, u32 b)
{
	syscall(SYSCALL_DEBUG_PRINT, fmt, a, b);
}


void syscall_debug_log2(const char* fmt, u32 a)
{
	syscall(SYSCALL_DEBUG_PRINT, fmt, a);
}

void syscall_debug_log(const char* fmt)
{
	syscall(SYSCALL_DEBUG_PRINT, fmt);
}

u32 syscall_process_pid(void)
{
	return syscall(SYSCALL_PROCESS_PID);
}

u32 syscall_process_uid(void)
{
	return syscall(SYSCALL_PROCESS_UID);
}

u32 syscall_process_gid(void)
{
	return syscall(SYSCALL_PROCESS_GID);
}

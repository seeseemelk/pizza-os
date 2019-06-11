#ifndef SYSCALL_HPP_
#define SYSCALL_HPP_

#include <cstdint>

namespace Syscall
{
	void handle_syscall();
	void initialise();

	// Common syscalls.
	u32 exit(u32, u32, u32);

	// VFS syscalls.
	namespace VFS
	{
		u32 is_open(u32, u32, u32);
		u32 get_buffer(u32, u32, u32);
	}

	// Process syscalls.
	namespace Process
	{
		u32 pid(u32, u32, u32);
		u32 uid(u32, u32, u32);
		u32 gid(u32, u32, u32);
		u32 yield(u32, u32, u32);
	}

	// I/O syscalls.
	namespace IO
	{
		void disable_port(u16 port);
	}

	// Ramdisk syscalls.
	namespace Ramdisk
	{
		u32 open(u32, u32, u32);
		u32 close(u32, u32, u32);
	}

	namespace Debug
	{
		u32 print(u32, u32, u32);
	}
}

#endif /* SYSCALL_HPP_ */

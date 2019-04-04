#ifndef SYSCALL_HPP_
#define SYSCALL_HPP_

#include <cstdint>

namespace Syscall
{
	void handle_syscall();

	// Common syscalls.
	u32 exit(u32, u32, u32);

	// Ramdisk syscalls.
	namespace Ramdisk
	{
		u32 open(u32, u32, u32);
		u32 close(u32, u32, u32);
	}
}

#endif /* SYSCALL_HPP_ */

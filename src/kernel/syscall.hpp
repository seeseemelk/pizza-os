#ifndef SYSCALL_HPP_
#define SYSCALL_HPP_

#include <cstdint>

namespace Syscall
{
	void handle_syscall();

	// Common syscalls.
	u32 exit(u32, u32, u32);

	// Ramdisk syscalls.

}

#endif /* SYSCALL_HPP_ */

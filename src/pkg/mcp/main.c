#include "../libc/include/pizzaos/syscall.h"

void main()
{
	syscall_debug_log("Checking if we're are process 1");
	if (syscall_process_pid() != 1)
	{
		syscall_debug_log2("We are process: %d", syscall_process_pid());
		syscall_exit();
	}

	syscall_debug_log("Checking if FD 0 is open");
	bool result = syscall_vfs_is_open(0);
	if (result == true)
		syscall_debug_log2("FD 0 is open", result);
	else
		syscall_debug_log2("FD 0 is closed", result);
	syscall_exit();

	while (1)
	{
		syscall(0xAAAAAAAA, 0xBBBBBBBB, 0xCCCCCCCC, 0xDDDDDDDD);
	}
}

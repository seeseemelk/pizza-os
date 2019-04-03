#include <pizzaos/syscall.h>

void syscall_exit(void)
{
	syscall(0);
}

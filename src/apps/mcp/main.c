#include <pizzaos/syscall.h>

void main()
{
	syscall_exit();
	while (1)
	{
		//syscall(0xAAAAAAAA, 0xBBBBBBBB, 0xCCCCCCCC, 0xDDDDDDDD);
	}
}

#include "syscall.hpp"
#include "process.hpp"
#include "debug.hpp"

u32 Syscall::exit(u32 ebx, u32 ecx, u32 edx)
{
	UNUSED(ebx); UNUSED(ecx); UNUSED(edx);
	log("Called exit");
	Proc::current_process->kill();
	return 0;
}

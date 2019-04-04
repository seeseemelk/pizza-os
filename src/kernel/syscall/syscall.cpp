#include "syscall.hpp"
#include "process.hpp"

typedef u32(*SyscallHandler)(u32, u32, u32);

SyscallHandler syscall_handlers[] = {Syscall::exit};

void Syscall::handle_syscall()
{
	u32 eax = Proc::current_process->peek_stack_end(-6);
	u32 ebx = Proc::current_process->peek_stack_end(-9);
	u32 ecx = Proc::current_process->peek_stack_end(-7);
	u32 edx = Proc::current_process->peek_stack_end(-8);
	log("Syscall time!");
	log("EAX = 0x%X", eax);
	log("EBX = 0x%X", ebx);
	log("ECX = 0x%X", ecx);
	log("EDX = 0x%X", edx);

	if (eax < sizeof(syscall_handlers) / sizeof(SyscallHandler))
	{
		syscall_handlers[eax](ebx, ecx, edx);
	}
}

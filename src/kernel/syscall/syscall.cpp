#include "syscall.hpp"
#include "process.hpp"
#include "debug.hpp"
#include "cpu.hpp"
#include <syscall_nums.h>

typedef u32(*SyscallHandler)(u32, u32, u32);

SyscallHandler syscall_handlers[256] {0};

void Syscall::initialise()
{
	syscall_handlers[SYSCALL_EXIT] = Syscall::exit;
	syscall_handlers[SYSCALL_VFS_ISOPEN] = Syscall::VFS::is_open;
	syscall_handlers[SYSCALL_PROCESS_PID] = Syscall::Process::pid;
	syscall_handlers[SYSCALL_PROCESS_GID] = Syscall::Process::gid;
	syscall_handlers[SYSCALL_PROCESS_UID] = Syscall::Process::uid;
	syscall_handlers[SYSCALL_PROCESS_YIELD] = Syscall::Process::yield;
	syscall_handlers[SYSCALL_DEBUG_PRINT] = Syscall::Debug::print;
}

void Syscall::handle_syscall()
{
	u32 eax = Proc::current_process->peek_stack_end(-6);
	u32 ebx = Proc::current_process->peek_stack_end(-9);
	u32 ecx = Proc::current_process->peek_stack_end(-7);
	u32 edx = Proc::current_process->peek_stack_end(-8);
	/*
	log("Syscall time!");
	log("EAX = 0x%X", eax);
	log("EBX = 0x%X", ebx);
	log("ECX = 0x%X", ecx);
	log("EDX = 0x%X", edx);
	//*/

	if (eax < sizeof(syscall_handlers) / sizeof(SyscallHandler))
	{
		SyscallHandler handler = syscall_handlers[eax];
		if (handler != nullptr)
		{
			log("Syscall 0x%X executed", eax);
			u32 result = handler(ebx, ecx, edx);
			Proc::current_process->poke_stack_end(-6, result);
		}
		else
		{
			log("Bad syscall 0x%X", eax);
			CPU::hang();
		}
	}
	else
	{
		log("Bad syscall 0x%X", eax);
		CPU::hang();
	}
}

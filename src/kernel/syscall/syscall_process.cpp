#include "syscall.hpp"
#include "process.hpp"
#include "debug.hpp"
#include "test.hpp"
#include <syscall_nums.h>

using namespace Syscall;

u32 Syscall::Process::pid(u32 a, u32 b, u32 c)
{
	UNUSED(a); UNUSED(b); UNUSED(c);
	return Proc::current_process->m_pid;
}

u32 Syscall::Process::uid(u32 a, u32 b, u32 c)
{
	UNUSED(a); UNUSED(b); UNUSED(c);
	return Proc::current_process->m_uid;
}

u32 Syscall::Process::gid(u32 a, u32 b, u32 c)
{
	UNUSED(a); UNUSED(b); UNUSED(c);
	return Proc::current_process->m_gid;
}

TEST(OTHER, "syscall debug", {
	Proc::Process* proc = require(Proc::exec_empty());
	assertEquals("Expected syscall to return PID", proc->m_pid, testSyscall(SYSCALL_PROCESS_PID));
	proc->m_pid++;
	assertEquals("Expected syscall to return PID", proc->m_pid, testSyscall(SYSCALL_PROCESS_PID));
});

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

u32 Syscall::Process::yield(u32 a, u32 b, u32 c)
{
	UNUSED(a); UNUSED(b); UNUSED(c);
	return 0;
}

TEST(OTHER, "syscall PROCESS_PID", {
	Proc::Process* proc = require(Proc::exec_empty());
	proc->m_pid = 0xDEADBEEF;
	assertEquals("Expected syscall to return PID (A)", proc->m_pid, testSyscall(SYSCALL_PROCESS_PID));
	proc->m_pid++;
	assertEquals("Expected syscall to return PID (B)", proc->m_pid, testSyscall(SYSCALL_PROCESS_PID));
});

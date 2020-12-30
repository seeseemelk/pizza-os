#include "syscall.hpp"

#include "debug.hpp"
#include "process.hpp"
#include "test.hpp"

u32 Syscall::Debug::print(u32 fmt, u32 a, u32 b)
{
	char* fmt_str = reinterpret_cast<char*>(fmt);
	::Debug::printf("[PROCESS:%d] ", Proc::current_process->m_pid);
	::Debug::printf(fmt_str, a, b);
	::Debug::printf("\n");
	return 0;
}

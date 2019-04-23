#include "process.hpp"

using namespace Proc;

u32 Process::peek_stack_end(size_t index)
{
	return m_syscall_stack[(sizeof(m_syscall_stack) / sizeof(u32)) + index];
}


void Process::poke_stack_end(size_t index, u32 value)
{
	m_syscall_stack[(sizeof(m_syscall_stack) / sizeof(u32)) + index] = value;
}

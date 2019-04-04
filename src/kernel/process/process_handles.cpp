#include "process.hpp"

using namespace Proc;

void Process::open_handle()
{
	m_count_handles++;
}

void Process::close_handle()
{
	if (--m_count_handles == 0)
	{
		allocator.free(*this);
	}
}

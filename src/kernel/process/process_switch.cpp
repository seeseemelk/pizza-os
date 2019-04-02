#include "process.hpp"
#include "debug.hpp"

using namespace Proc;

bool Process::is_current()
{
	return this == Proc::current_process;
}

void Process::switch_to()
{
	if (!is_current())
	{
		log("Reloading pagetable");
		CPU::set_ring3_syscall_stack(m_syscall_stack + 24);
		load_pagetable();
	}
	else
		log("Already in current process");
}

void Process::load_pagetable()
{
	Paging::load_directory(m_page_directory);
}

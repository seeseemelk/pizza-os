#include "process.hpp"

using namespace Processes;

bool Process::is_current()
{
	return this == Processes::current_process;
}

void Process::switch_to()
{
	if (!is_current())
	{
		load_pagetable();
	}
}

void Process::load_pagetable()
{
	Paging::load_directory(m_page_directory);
}

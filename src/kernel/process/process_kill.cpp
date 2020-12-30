#include "process.hpp"

#include "kernel.hpp"
#include "paging.hpp"
#include "pmem.hpp"
#include "vfs.hpp"

using namespace Proc;

static void free_memory(Paging::PageTableEntry& entry)
{
	size_t phys = entry.get_address();
	entry.present = 0;
	PMem::free(phys);
}

static void free_memory(Paging::PageDirEntry& entry)
{
	Paging::PageTable& table = entry.get_table();
	for (int i = 0; i < 1024; i++)
	{
		Paging::PageTableEntry& entry = table.entries[i];
		if (entry.present)
			free_memory(entry);
	}

	entry.present = 0;
	PMem::free(entry.get_address());
}

static void free_memory()
{
	for (size_t addr = 0; addr < Kernel::kernel_start; addr += KB(4))
	{
		Paging::PageDirEntry& entry = Paging::directory.get_entry(reinterpret_cast<void*>(addr));
		if (entry.present)
			free_memory(entry);
	}
}

static void unlink_process(Process* process)
{
	Process* next = process->m_next_process;
	Process* previous = process->m_previous_process;

	next->m_previous_process = previous;
	previous->m_next_process = next;
}

void Process::kill()
{
	Process* process = current_process;

	if (process == process->m_next_process)
	{
		// This process is the very last process to kill.
		// We don't have to free the memory as there won't be
		// anything using it after this function returns.
		process->m_state = ProcessState::DEAD;
	}
	else
	{
		free_memory();
		process->m_next_process->switch_to();
		PMem::free(process->m_page_directory);
		unlink_process(process);
		process->m_state = ProcessState::DEAD;

		if (process->m_count_handles == 0)
			allocator.free(*this);

		VFS::free_process();
	}
}

#include "process.hpp"
#include "paging.hpp"
#include "debug.hpp"
#include "cpu.hpp"

using namespace Processes;

Slab<Process> Processes::allocator;
Process* Processes::current_process;

static void init_allocator()
{
	Result<Paging::PageTable*> table = Paging::alloc_table();
	if (table.is_fail())
	{
		log("Failed to allocate a pagetable for the process allocator");
		CPU::out_of_memory();
	}
	allocator.init(*table.result);
}

static void init_first_process()
{
	Result<Process*> result = allocator.alloc();
	if (result.is_fail())
	{
		log("Failed to allocate first userspace process");
		CPU::out_of_memory();
	}
	current_process = result.result;
	current_process->m_pid = static_cast<int>(allocator.index_of(*current_process));
	current_process->m_uid = 0;
	current_process->m_gid = 0;
	current_process->m_state = ProcessState::STARTING;
}

void Processes::init()
{
	init_allocator();
	init_first_process();
}

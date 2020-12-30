#include "process.hpp"

#include "cpu.hpp"
#include "debug.hpp"
#include "paging.hpp"

using namespace Proc;

Slab<Process> Proc::allocator;
Process* Proc::current_process = 0;
u8* Proc::process_local_page;
unsigned int Proc::process_local_index = 0;

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

static void init_local_page()
{
	Result<Paging::PageTableEntry*> result = Paging::alloc_table_entry();
	if (result.is_fail())
	{
		log("Failed to allocate process-local page");
		CPU::out_of_memory();
	}

	Paging::PageTableEntry* entry = result.result;
	if (entry->alloc_any_memory() == ResultState::FAIL)
	{
		log("Failed to allocate memory for process-local page");
		CPU::out_of_memory();
	}
	process_local_page = static_cast<u8*>(entry->get_virtual_address());
}

void Proc::init()
{
	init_allocator();
	init_local_page();
}

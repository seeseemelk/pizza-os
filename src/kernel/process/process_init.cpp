#include "process.hpp"
#include "paging.hpp"
#include "debug.hpp"
#include "cpu.hpp"

using namespace Proc;

Slab<Process> Proc::allocator;
Process* Proc::current_process = 0;
unsigned int Proc::num_processes = 0;

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

void Proc::init()
{
	init_allocator();
}

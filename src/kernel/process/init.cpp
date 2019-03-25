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
	current_process->pid = static_cast<int>(allocator.index_of(*current_process));
	current_process->uid = 0;
	current_process->gid = 0;
	current_process->state = ProcessState::STARTING;
}

static void test_process()
{
	if (current_process->map_page(reinterpret_cast<void*>(0x200)) == ResultState::FAIL)
	{
		log("Could not allocate memory for process");
		CPU::out_of_memory();
	}

	/*char* ptr = reinterpret_cast<char*>(0x200);
	ptr[0] = 'H';
	ptr[1] = 'w';
	ptr[2] = '\0';*/
}

void Processes::init()
{
	init_allocator();
	init_first_process();

	test_process();
}

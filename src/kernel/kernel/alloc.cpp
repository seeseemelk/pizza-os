#include "kernel.hpp"
#include "kernel/alloc.hpp"
#include "cpu.hpp"
#include "debug.hpp"
#include "paging.hpp"
#include "result.hpp"

using namespace Kernel;

static Paging::PageTable* pagetable;
static void* alloc_map;
static size_t alloc_index = 0;
static size_t alloced = 0;

void Kernel::init_alloc()
{
	Paging::PageDirEntry* dir_entry;
	Result<Paging::PageTable*> result = Paging::alloc_table(&dir_entry);
	if (result.is_fail())
		CPU::out_of_memory();

	alloc_map = dir_entry->get_virtual_address();
	pagetable = result.result;
}

Result<void*> Kernel::alloc(size_t bytes)
{
	void* address = alloc_map;
	alloc_map += bytes;

	while (alloc_index > alloced)
	{
		if (pagetable->entries[alloced / 1024].alloc_any_memory() == ResultState::FAIL)
			return Result<void*>();
	}
	return Result<void*>(address);
}

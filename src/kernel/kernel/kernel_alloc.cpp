#include "kernel.hpp"

#include "cpu.hpp"
#include "debug.hpp"
#include "kernel/alloc.hpp"
#include "libc/string.hpp"
#include "paging.hpp"
#include "result.hpp"

using namespace Kernel;

static Paging::PageTable* pagetable;
static u8* alloc_map;
static size_t alloc_index = 0;
static size_t alloced = 0;

void Kernel::init_alloc()
{
	Paging::PageDirEntry* dir_entry;
	Result<Paging::PageTable*> result = Paging::alloc_table(&dir_entry);
	if (result.is_fail())
		CPU::out_of_memory();

	//memset(dir_entry, 0, sizeof(Paging::PageDirEntry));
	alloc_map = static_cast<u8*>(dir_entry->get_virtual_address());
	pagetable = result.result;
}

Result<void*> Kernel::alloc(size_t bytes)
{
	if (alloc_index >= MB(4))
	{
		log("Too much allocated using Kernel::alloc");
		CPU::out_of_memory();
	}

	void* address = alloc_map + alloc_index;
	alloc_index += bytes;

	while (alloc_index > alloced)
	{
		size_t tbl_index = Paging::tbl_index(reinterpret_cast<void*>(alloced));
		Paging::PageTableEntry& entry = pagetable->entries[tbl_index];
		if (entry.alloc_any_memory() == ResultState::FAIL)
			return Result<void*>();
		else
		{
			entry.present = 1;
			entry.writable = 1;
		}
		alloced += KB(4);
	}
	return Result<void*>(address);
}

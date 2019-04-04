#include "vfs.hpp"
#include "paging.hpp"
#include "result.hpp"
#include "debug.hpp"

using namespace VFS;

Slab<File> VFS::allocator;
Paging::PageTableEntry* VFS::process_pagetable;

void VFS::init()
{
	Result<Paging::PageTable*> tableResult = Paging::alloc_table();

	if (tableResult.is_fail())
	{
		log("Failed to allocate page for VFS.");
		CPU::hang();
	}

	allocator.init(*tableResult.result);

	Result<Paging::PageTableEntry*> entryResult = Paging::alloc_table_entry();
	if (entryResult.is_fail())
	{
		log("Failed to allocate page for process-memory VFS");
		CPU::hang();
	}

	process_pagetable = entryResult.result;
}

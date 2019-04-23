#include "vfs.hpp"
#include "paging.hpp"
#include "result.hpp"
#include "debug.hpp"

using namespace VFS;

Slab<File>* VFS::allocator;
Paging::PageTableEntry* VFS::process_pagetable;
File* VFS::descriptors;
FileBuffer* VFS::buffers;

void VFS::init()
{
	allocator = Process::allocate_local(sizeof(Slab<File>*));

	// We allocate an entire page directory to store process-local VFS information.
	/*Result<Paging::PageTable*> tableResult = Paging::alloc_table();

	if (tableResult.is_fail())
	{
		log("Failed to allocate page for VFS.");
		CPU::hang();
	}

	allocator.init(*tableResult.result);*/

	// We also allocate several bytes in process-local memory.
	// These bytes will contain the slab allocator itself.
	/*Result<Paging::PageTableEntry*> entryResult = Paging::alloc_table_entry();
	if (entryResult.is_fail())
	{
		log("Failed to allocate page for process-memory VFS");
		CPU::hang();
	}

	process_pagetable = entryResult.result;*/
}

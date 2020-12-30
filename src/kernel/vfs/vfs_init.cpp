#include "vfs.hpp"
#include "vfs/vfs_private.hpp"

#include "cpu.hpp"
#include "debug.hpp"
#include "paging.hpp"
#include "result.hpp"

using namespace VFS;

Slab<File>* VFS::allocator;
Paging::PageTable* VFS::file_page;
Paging::PageTable* VFS::buffer_page;
/*File* VFS::descriptors;
FileBuffer* VFS::buffers;*/

static void allocate_page(Paging::PageTable*& table)
{
	Paging::PageDirEntry& dir_entry = Paging::alloc_dir_entry();
	Result<Paging::PageTable*> result = dir_entry.make_table();
	if (result.is_fail())
	{
		log("Failed to allocate page table for VFS functions");
		CPU::hang();
	}

	table = result.result;
}

void VFS::init()
{
	log("Allocating process-local VFS file slab allocator");
	Result<Slab<File>*> result = Proc::allocate_local<Slab<File>*>(sizeof(Slab<File>*));
	if (result.is_fail())
	{
		log("Failed to allocate VFS allocator");
		CPU::hang();
	}

	allocator = result.result;

	log("Allocating VFS file page table");
	allocate_page(file_page);

	log("Allocating VFS buffer page table");
	allocate_page(buffer_page);

	log("Initialising mounts system");
	init_mounts();

	log("Global VFS structures initialised");
}

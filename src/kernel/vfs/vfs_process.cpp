#include "vfs.hpp"
#include "paging.hpp"
#include "debug.hpp"
#include "cpu.hpp"
#include "pmem.hpp"

using namespace VFS;

void VFS::init_process()
{
	if (process_pagetable->alloc_any_memory() == ResultState::FAIL)
	{
		log("Failed to allocate process-memory file list");
		CPU::hang();
	}
}

void VFS::free_process()
{
	// TODO Free file descriptors

	// Free process-memory file list");
	PMem::free(process_pagetable->get_address());
}

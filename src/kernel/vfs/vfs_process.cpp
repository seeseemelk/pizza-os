#include "vfs.hpp"
#include "paging.hpp"
#include "debug.hpp"
#include "cpu.hpp"
#include "pmem.hpp"

using namespace VFS;

void VFS::init_process()
{
	log("Creating process-local VFS structures");
	allocator->init(*file_page);
	log("Finished creating process-local VFS structures");
}

void VFS::free_process()
{
	// TODO Free file descriptors

	// Free process-memory file list");
	//PMem::free(process_pagetable->get_address());
}

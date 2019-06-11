#include "vfs.hpp"
#include "vfs/vfs_private.hpp"
#include "slab.hpp"
#include "paging.hpp"
#include "result.hpp"
#include "cpu.hpp"
#include "debug.hpp"
#include "safe.hpp"

using namespace VFS;

Mount* last_mount = nullptr;
Slab<Mount> mount_allocator;

void VFS::init_mounts()
{
	Result<Paging::PageTable*> result = Paging::alloc_table();
	if (result.is_fail())
	{
		log("Failed to allocate pagetable for mount allocator");
		CPU::hang();
	}

	Paging::PageTable* table = result.result;
	mount_allocator.init(*table);
}

ResultState VFS::mount(char* path, Proc::Process* process)
{
	Result<Mount*> result = mount_allocator.alloc();
	if (result.is_fail())
	{
		log("Failed to allocate memory for mount");
		return ResultState::FAIL;
	}
	Mount* mount = result.result;

	Safe::SafeResult safeResult;

	if ((safeResult = Safe::strncpy(mount->path, path, PATH_MAX)) != Safe::SafeResult::OK)
	{
		log("Path copy failed: %s", Safe::describe(safeResult));
		return ResultState::FAIL;
	}

	mount->fs_host = process;
	return ResultState::SUCCESS;
}

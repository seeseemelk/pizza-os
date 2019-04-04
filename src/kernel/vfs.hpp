#ifndef VFS_HPP_
#define VFS_HPP_

#include "process.hpp"
#include "slab.hpp"

namespace VFS
{
	enum FileType
	{
		FILE,
		CHAR,
		PIPE,
		RAMDISK
	};

	struct File
	{
		Proc::Process* process;
		FileType type;
	};

	extern Slab<File> allocator;
	extern Paging::PageTableEntry* process_pagetable;

	void init();

	void init_process();
	void free_process();

	File& allocate_file();
	void free_file(File& file);
}

#endif

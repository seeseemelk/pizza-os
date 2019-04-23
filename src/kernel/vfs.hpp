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

	struct FileBuffer
	{
		u32 read_index;
		u32 write_ptr;
		u8 content[4096-sizeof(u32)*2];
	};

	struct ProcessFileBuffers
	{
		FileBuffer buffers[4096];
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

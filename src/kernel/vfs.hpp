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

	enum FileState
	{
		CLOSED,
		OPEN
	};

	struct File
	{
		//Proc::Process* process;
		FileType type;
		FileState state;
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

	extern Slab<File>* allocator;
	extern Paging::PageTable* file_page;
	extern Paging::PageTable* buffer_page;

	void init();

	void init_process();
	void free_process();

	Result<File*> get_file(unsigned int fd);
	bool is_open(unsigned int fd);

	File& allocate_file();
	void free_file(File& file);
}

#endif

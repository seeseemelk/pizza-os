#include "vfs.hpp"

#include "process.hpp"

using namespace VFS;

bool VFS::is_open(unsigned int fd)
{
	UNUSED(fd);
	return false;
}

// This is not compatible with the process-local file allocator.
/*
Result<File*> VFS::get_file(unsigned int fd)
{
	if (fd >= Proc::current_process->m_num_fd
			|| fd >= VFS::MAX_FILES)
		return Result<File*>();
	else
		return descriptors[fd];
}

Result<File*> VFS::make_file(unsigned int fd)
{
	return Result<File*>();
}

bool VFS::is_open(unsigned int fd)
{
	Result<File*> file = get_file(fd);
	if (file.is_fail())
		return false;
	else
		return file.result->state != FileState::CLOSED;
}
*/

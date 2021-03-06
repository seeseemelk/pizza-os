#include "ramdisk.hpp"

#include "debug.hpp"
#include "libc/math.hpp"
#include "libc/stddef.hpp"
#include "libc/stdint.hpp"
#include "libc/string.hpp"

using namespace RamDisk;

const TarFile* first_file;
size_t num_files = 0;

static bool is_zero_block(const void* buf)
{
	const u32* u32_buf = static_cast<const u32*>(buf);
	for (size_t i = 0; i < 512/sizeof(u32); i++)
	{
		if (u32_buf[i] != 0)
			return false;
	}
	return true;
}

static const TarFile* next_file(const TarFile* file)
{
	const u8* file_addr = reinterpret_cast<const u8*>(file);
	size_t file_size = ceilg(file->get_size(), 0x200);
	file_addr += 512 + file_size;
	return reinterpret_cast<const TarFile*>(file_addr);
}

void RamDisk::init_ramdisk(const void* ramdisk, size_t length)
{
	first_file = static_cast<const TarFile*>(ramdisk);
	const TarFile* file = first_file;

	log("Listing ramdisk contents:");
	while (!is_zero_block(file))
	{
		num_files++;
		length -= sizeof(TarFile) + file->get_size();
		size_t address = reinterpret_cast<size_t>(file) - reinterpret_cast<size_t>(first_file);
		log("(0x%X) %s: %d bytes", address, file->filename, file->get_size());
		file = next_file(file);
	}

	if (!is_zero_block(file))
		log("Encountered end of tar file prematurely");

	log("Total: %d files", num_files);
}

bool RamDisk::has_file(const char* filename)
{
	const TarFile* file = first_file;
	for (size_t i = 0; i < num_files; i++)
	{
		if (strncmp(filename, file->filename, 100) == 0)
			return true;
		file = next_file(file);
	}
	return false;
}

Result<const TarFile*> RamDisk::get_file(const char* filename)
{
	const TarFile* file = first_file;
	for (size_t i = 0; i < num_files; i++)
	{
		if (strncmp(filename, file->filename, 100) == 0)
			return Result<const TarFile*>(file);
		file = next_file(file);
	}
	return Result<const TarFile*>();
}
































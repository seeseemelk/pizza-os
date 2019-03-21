#include <cstdint>
#include "ramdisk.hpp"

using namespace RamDisk;

size_t TarFile::get_size() const
{
	size_t size = 0;
	for (int i = 0; i < 12; i++)
	{
		if (this->size[i] == 0)
			break;
		size = (size*8) + (this->size[i] - '0');
	}
	return size;
}

const char* TarFile::buffer() const
{
	// The contents of the file start right after the header.
	return reinterpret_cast<const char*>(this) + 0x200;
}
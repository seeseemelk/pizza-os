#include "multiboot.hpp"
#include "debug.hpp"
#include <cstdint>

using namespace Multiboot;

size_t msize = 0;

/*
static void parse_mmap()
{
	u32 length = mbt->mmap_length;
	multiboot_mmap_entry* entries = reinterpret_cast<multiboot_mmap_entry*>(mbt->mmap_addr);
	log("Entries length: %d", length);
}

size_t Multiboot::memory_size()
{
	if (msize == 0)
		parse_mmap();
	return msize;
}
*/

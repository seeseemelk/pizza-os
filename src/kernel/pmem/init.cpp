#include "pmem.hpp"
#include "debug.hpp"
#include "multiboot.hpp"
#include <cstddef>
#include <cstdint>

using namespace PMem;

static size_t total_size()
{
	size_t size = 0;

	size_t length = Multiboot::mbt->mmap_length;
	size_t entry_addr = Multiboot::mbt->mmap_addr;
	size_t entry_end = entry_addr + length;

	while (entry_addr < entry_end)
	{
		multiboot_mmap_entry* entry = reinterpret_cast<multiboot_mmap_entry*>(entry_addr);
		size += static_cast<size_t>(entry->len);
		entry_addr += entry->size + sizeof(u32);
		length -= entry->size;
	}

	return size;
}

void PMem::init()
{
	size_t mem_length = total_size();
	if (mem_length < KB(64))
		log("Total memory size: %d B", mem_length);
	else if (mem_length < MB(4))
		log("Total memory size: %d KiB", mem_length / 1024);
	else
		log("Total memory size: %d MiB", mem_length / 1024 / 1024);
}

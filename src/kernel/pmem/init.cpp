#include "pmem.hpp"
#include "debug.hpp"
#include "multiboot.hpp"
#include "cpu.hpp"
#include <cstddef>
#include <cstdint>
#include <cmath>

using namespace PMem;

#define KERNEL_END reinterpret_cast<size_t>(&kernel_end);
extern u8 kernel_end;

//u8* PMem::map = reinterpret_cast<u8*>(-1);
u8 PMem::map[4080];
size_t PMem::map_length = 4080;

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

static void allocate_map()
{
	/*size_t length = Multiboot::mbt->mmap_length;
	size_t entry_addr = Multiboot::mbt->mmap_addr;
	size_t entry_end = entry_addr + length;

	while (entry_addr < entry_end)
	{
		multiboot_mmap_entry* entry = reinterpret_cast<multiboot_mmap_entry*>(entry_addr);

		size_t len = static_cast<size_t>(entry->len);
		//size_t addr_start = static_cast<size_t>(entry->addr);
		size_t addr_end = static_cast<size_t>(entry->addr);
		size_t addr_start = KERNEL_END;
		addr_start -= 0xC0000000;
		if (addr_end > addr_start)
		{
			len = addr_end - addr_start;
			if (entry->type == MULTIBOOT_MEMORY_AVAILABLE && len >= map_length && addr_end < MB(4))
			{
				log("Found something at 0x%X (%d, %d)", static_cast<size_t>(entry->addr), entry->type, static_cast<size_t>(entry->len));
				map = reinterpret_cast<u8*>(entry->addr);
				log("pmmap allocated at 0x%X", map);
				return;
			}
		}
		entry_addr += entry->size + sizeof(u32);
	}

	log("Error: no large enough memory hole found");
	CPU::hang();*/
}

static void zero_map()
{
	for (size_t i = 0; i < map_length; i++)
		map[i] = BlockState::FREE;
	log("pmmap zeroed");
}

static void populate_map()
{
	size_t length = Multiboot::mbt->mmap_length;
	size_t entry_addr = Multiboot::mbt->mmap_addr;
	size_t entry_end = entry_addr + length;

	while (entry_addr < entry_end)
	{
		multiboot_mmap_entry* entry = reinterpret_cast<multiboot_mmap_entry*>(entry_addr);

		size_t len = static_cast<size_t>(entry->len);
		size_t addr = static_cast<size_t>(entry->addr);

		if (entry->type != MULTIBOOT_MEMORY_AVAILABLE)
			set_state(addr, len, BlockState::RESERVED);

		entry_addr += entry->size + sizeof(u32);
		length -= entry->size;
	}
	log("pmmap populated");
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

	map_length = mem_length / KB(4);
	log("Need %d KiB for pmmap", map_length / 1024);

	allocate_map();
	zero_map();
	populate_map();
}
















#include "pmem.hpp"
#include "debug.hpp"
#include "multiboot.hpp"
#include "cpu.hpp"
#include "paging.hpp"
#include "kernel.hpp"
#include "result.hpp"
#include <cstddef>
#include <cstdint>
#include <cmath>

using namespace PMem;

#define KERNEL_END reinterpret_cast<size_t>(&kernel_end);
extern u8 kernel_end;

typedef multiboot_mmap_entry MemEntry;
Paging::PageTable pagetable;
size_t total_memory = 0;
size_t PMem::map_length = 0;
u8* PMem::map;

static void setup_pagetable()
{
	for (size_t i = 0; i < 1024; i++)
	{
		reinterpret_cast<u32&>(pagetable.entries[i]) = 0;
	}
}

static MemEntry& get_next_entry(MemEntry& entry)
{
	void* entry_ptr = &entry;
	void* new_entry_ptr = entry_ptr + entry.size + 4;
	return *reinterpret_cast<MemEntry*>(new_entry_ptr);
}

static bool add_pagetable_entries_for_mementry(MemEntry& entry, size_t length_left, int& pagetable_end)
{
	length_left -= entry.size + 4;
	total_memory += entry.len;

	// Check if we need to add an entry.
	MemEntry& next_entry = get_next_entry(entry);
	if (length_left > 0 && add_pagetable_entries_for_mementry(next_entry, length_left, pagetable_end) == false)
		return false;

	// Add the entry.
	size_t first_area = ceilg(entry.addr, KB(4));
	size_t last_area = floorg(entry.addr + entry.len - 1, KB(4));

	// These blocks refer to blocks of 4096 states.
	size_t blocks_needed = ceildiv(total_memory, MB(4)) - map_length;
	size_t num_blocks = (last_area - first_area) / KB(4);
	size_t num_blocks_to_allocate_now = (num_blocks < blocks_needed) ? num_blocks : blocks_needed;

	//size_t end_block = first_block + num_blocks_to_allocate_now * KB(4);
	//for (size_t block = first_block; block < end_block; block += KB(4))
	for (size_t block = 0; block < num_blocks_to_allocate_now; block++)
	{
		Paging::PageTableEntry& entry = pagetable.entries[pagetable_end++];
		entry.set_address(first_area + block * KB(4));
		entry.writable = 1;
		entry.present = 1;
	}

	blocks_needed -= num_blocks_to_allocate_now * 4096;
	map_length += num_blocks_to_allocate_now * 4096;

	return blocks_needed > 0;
}

static void add_pagetable_entries()
{
	size_t length = Multiboot::mbt->mmap_length;
	size_t entry_addr = Multiboot::mbt->mmap_addr;
	MemEntry& entry = *reinterpret_cast<MemEntry*>(entry_addr);
	int pagetable_end = 0;
	if (add_pagetable_entries_for_mementry(entry, length, pagetable_end) == true)
		CPU::out_of_memory();
	//size_t entry_end = entry_addr + length;

}

void PMem::init()
{
	Paging::PageDirEntry& entry = Paging::alloc_dir_entry();
	entry.set_address(reinterpret_cast<size_t>(&pagetable) - GB(3));
	map = static_cast<u8*>(entry.get_virtual_address());
	setup_pagetable();
	add_pagetable_entries();
	log("PMem allocator ready");
}

//u8* PMem::map = reinterpret_cast<u8*>(-1);
//u8 PMem::map[4080];
//size_t PMem::map_length = 4080;
/*
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
	CPU::hang();*/ /*
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
*/


































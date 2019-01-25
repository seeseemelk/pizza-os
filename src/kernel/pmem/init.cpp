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
#include <cstring>

using namespace PMem;

extern u8 _kernel_start;
extern u8 _kernel_end;

static const u32 kernel_start = reinterpret_cast<size_t>(&_kernel_start);
static const u32 kernel_end = reinterpret_cast<size_t>(&_kernel_end);
static const u32 kernel_size = kernel_end - kernel_start;

typedef multiboot_mmap_entry MemEntry;
static Paging::PageTable pagetable;
static size_t total_memory = 0;
size_t PMem::map_length = 0;
u8* PMem::map;

static void setup_pagetable()
{
	memset(&pagetable, 0, sizeof(Paging::PageTable));
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

	if (entry.type == MULTIBOOT_MEMORY_AVAILABLE)
	{
		size_t entry_end = entry.addr + entry.len;
		total_memory = (entry_end > total_memory) ? entry_end : total_memory;
	}

	// Check if we need to add an entry.
	MemEntry& next_entry = get_next_entry(entry);
	if (length_left > 0 && add_pagetable_entries_for_mementry(next_entry, length_left, pagetable_end) == false)
		return false;

	// Add the entry.
	size_t first_area = ceilg(entry.addr, KB(4));
	size_t last_area = floorg(entry.addr + entry.len - 1, KB(4));

	// These blocks refer to areas of 4096 states.
	// Each state refers to a blob of memory of 4 KiB.
	// So each block refers to 4 KiB * 4096 = 16 MiB.
	size_t blocks_needed = ceildiv(total_memory, MB(16)) - map_length;

	if (entry.type == MULTIBOOT_MEMORY_AVAILABLE)
	{
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
		blocks_needed -= num_blocks_to_allocate_now;
	}

	//map_length += num_blocks_to_allocate_now * 4096;

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

}

static void reserve_map_entries()
{
	// Reserve memory from map entries.
	log("Reserving from multiboot structure");
	size_t length = Multiboot::mbt->mmap_length;
	size_t entry_addr = Multiboot::mbt->mmap_addr;
	MemEntry& entry = *reinterpret_cast<MemEntry*>(entry_addr);

	while (length > 0)
	{
		if (entry.type != MULTIBOOT_MEMORY_AVAILABLE)
			PMem::set_state(entry.addr, entry.len, BlockState::RESERVED);

		length -= entry.size + 4;
		entry = get_next_entry(entry);
	}

	// Reserve memory used by the kernel.
	const size_t kstart = kernel_start - GB(3);
	const size_t ksize = kernel_size;
	log("Reserving for kernel, 0x%X 0x%X", kstart, ksize);
	PMem::set_state(kstart, ksize, BlockState::RESERVED);

	// Reserve memory that is used by PMem::map.
	log("Reserving for PMem::map");
	int index = 0;
	Paging::PageTableEntry* pt_entry = pagetable.entries + index++;
	while (pt_entry->present)
	{
		size_t address = pt_entry->get_address();
		if (address < total_memory)
			PMem::set_state(address, KB(4), BlockState::RESERVED);
		pt_entry = pagetable.entries + index++;
	}
}

static void log_memory()
{
	if (total_memory <= MB(4))
		log("Total memory: %d KiB", total_memory / KB(1));
	else
		log("Total memory: %d MiB", total_memory / MB(1));
}

void PMem::init()
{
	Paging::PageDirEntry& entry = Paging::alloc_dir_entry();
	entry.set_address(reinterpret_cast<size_t>(&pagetable) - GB(3));
	entry.present = 1;
	entry.writable = 1;
	map = static_cast<u8*>(entry.get_virtual_address());
	setup_pagetable();
	add_pagetable_entries();
	map_length = total_memory / KB(4);
	reserve_map_entries();
	log_memory();
	log("PMem allocator ready");
}


































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
	void* new_entry_ptr = static_cast<u8*>(entry_ptr) + entry.size + 4;
	return *reinterpret_cast<MemEntry*>(new_entry_ptr);
}

static void process_area(size_t& blocks_needed, int& pagetable_end, size_t first_area, size_t last_area)
{
	first_area = ceilg(first_area, KB(4));
	last_area = floorg(last_area, KB(4));

	if (first_area >= last_area)
		return;

	size_t num_blocks = (last_area - first_area) / KB(4);
	size_t num_blocks_to_allocate_now = (num_blocks < blocks_needed) ? num_blocks : blocks_needed;
	for (size_t block = 0; block < num_blocks_to_allocate_now; block++)
	{
		Paging::PageTableEntry& entry = pagetable.entries[pagetable_end++];
		entry.set_address(first_area + block * KB(4));
		entry.writable = 1;
		entry.present = 1;
	}

	blocks_needed -= num_blocks_to_allocate_now;
}

/**
 * Some entries might consider the memory segment the kernel is in to be 'available'.
 * These segments we split into two different, the others we leave untouched.
 */
static void offset_for_kernel(size_t& blocks_needed, int& pagetable_end, size_t first_area, size_t last_area)
{
	static size_t phys_kernel_start = kernel_start - GB(3);
	static size_t phys_kernel_end = kernel_end - GB(3);
	if ((phys_kernel_start >= first_area && phys_kernel_start < last_area)
			|| (phys_kernel_end > last_area && phys_kernel_end <= last_area))
	{
		process_area(blocks_needed, pagetable_end, first_area, phys_kernel_start);
		process_area(blocks_needed, pagetable_end, phys_kernel_end, last_area);
	}
	else
		process_area(blocks_needed, pagetable_end, first_area, last_area);
}

static bool add_pagetable_entries_for_mementry(MemEntry& entry, size_t length_left, int& pagetable_end)
{
	length_left -= entry.size + 4;

	if (entry.type == MULTIBOOT_MEMORY_AVAILABLE)
	{
		auto entry_end = static_cast<size_t>(entry.addr + entry.len);
		total_memory = (entry_end > total_memory) ? entry_end : total_memory;
	}

	// Check if we need to add an entry.
	MemEntry& next_entry = get_next_entry(entry);
	if (length_left > 0 && !add_pagetable_entries_for_mementry(next_entry, length_left, pagetable_end))
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
		offset_for_kernel(blocks_needed, pagetable_end, first_area, last_area);
	}

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

static void clear_memory_map()
{
	memset(map, 0, map_length);
}

void PMem::init()
{
	log("kernel_start: 0x%X (phys: 0x%X)", kernel_start, kernel_start - GB(3));
	log("kernel_end: 0x%X (phys: 0x%X)", kernel_end, kernel_end - GB(3));

	// Get an address for the pagetable of the PMem allocator itself.
	// We can't using any other allocator at this point yet.
	Paging::PageDirEntry& entry = Paging::alloc_dir_entry();
	entry.set_address(reinterpret_cast<size_t>(&pagetable) - GB(3));
	entry.writable = 1;
	entry.present = 1;
	map = static_cast<u8*>(entry.get_virtual_address());
	log("Virtual address of memory map: 0x%X", map);
	setup_pagetable();

	add_pagetable_entries();
	map_length = total_memory / KB(4);
	clear_memory_map();
	reserve_map_entries();
	log_memory();
	log("PMem allocator ready");
}

void PMem::fix_pagetable()
{
	Paging::PageTableEntry& meta_entry = Paging::get_meta_entry(map);
	log("Meta entry: 0x%X", meta_entry.get_virtual_address());
	meta_entry.set_address(reinterpret_cast<size_t>(&pagetable) - GB(3));
	meta_entry.present = 1;
}
































#include "paging.hpp"
#include "debug.hpp"
#include "cpu.hpp"
#include "test.hpp"
#include "pmem.hpp"
#include <cmath>
#include <cstring>

using namespace Paging;

/**
 * The page directory used by the bootloader.
 */
extern "C" PageDirectory _system_page_directory;

/**
 * The page table used by the bootloader that maps to the lower 4 MiB.
 */
extern "C" PageTable _system_default_page_table;

PageDirectory& Paging::directory = _system_page_directory;
PageTable* const Paging::tables = reinterpret_cast<PageTable*>(GB(4) - MB(4));

static PageTable startup_metatable;
PageTable& Paging::metatable = *reinterpret_cast<PageTable*>(GB(4) - KB(4));

void Paging::init()
{
	log("Page directory address: 0x%X", &directory);
	log("Page tables: 0x%X", tables);

	log("Zeroing-out metatable");
	memset(static_cast<void*>(&startup_metatable), 0, sizeof(PageTable));

	log("Enabling metatable");
	PageDirEntry& mt = directory.entries[1023];
	mt.set_address(reinterpret_cast<size_t>(&startup_metatable) - 0xC0000000);
	mt.present = 1;
	mt.writable = 1;

	log("Making metatable self-referencing");
	PageTableEntry& mte = startup_metatable.entries[1023];
	mte.set_address(mt.get_address());
	mte.present = 1;
	mte.writable = 1;

	log("Adding startup page table to metatable");
	PageTableEntry& system_page_entry = get_meta_entry(reinterpret_cast<void*>(0xC0000000));
	system_page_entry.set_address(reinterpret_cast<size_t>(&_system_default_page_table) - 0xC0000000);
	system_page_entry.present = 1;
	system_page_entry.writable = 0;

	log("Has table: %b", directory.has_table(0));

	log("Disabling initial sub-4MiB pagetable");
	directory.get_entry(0).present = 0;

	PMem::fix_pagetable();
}

size_t Paging::dir_index(const void* virt)
{
	return reinterpret_cast<u32>(virt) / MB(4);
}

size_t Paging::tbl_index(const void* virt)
{
	return (reinterpret_cast<u32>(virt) % MB(4)) / KB(4);
}

PageTableEntry& Paging::get_meta_entry(const void* virt)
{
	size_t index = dir_index(virt);
	return metatable.entries[index];
}

void Paging::load_directory(u32 directory)
{
	// Make _system_default_page_table refer to the new directory.
	PageTableEntry& entry = _system_default_page_table.get_entry(reinterpret_cast<void*>(&_system_page_directory));
	entry.set_address(directory);

	// Load new CR3
	CPU::load_cr3(directory);
}

TEST(PRE_PAGING, "Paging::dir_index(0) -> 0",
{
	assertEquals("Expected a value of 0", 0, Paging::dir_index(0));
});

TEST(PRE_PAGING, "Paging::dir_index(0x0040_0000) -> 1",
{
	assertEquals("Expected a value of 1", 1, Paging::dir_index(reinterpret_cast<void*>(0x400000)));
});

TEST(PRE_PAGING, "Paging::dir_index(0xFFFF_FFFF) -> 1023",
{
	assertEquals("Expected a value of 1023", 1023, Paging::dir_index(reinterpret_cast<void*>(0xFFFFFFFF)));
});

TEST(PRE_PAGING, "Paging::tbl_index(0) -> 0",
{
	assertEquals("Expected a value of 0", 0, Paging::tbl_index(0));
});

TEST(PRE_PAGING, "Paging::tbl_index(0x0040_0000) -> 0",
{
	assertEquals("Expected a value of 0", 0, Paging::tbl_index(reinterpret_cast<void*>(0x400000)));
});

TEST(PRE_PAGING, "Paging::tbl_index(0x0040_1000) -> 1",
{
	assertEquals("Expected a value of 1", 1, Paging::tbl_index(reinterpret_cast<void*>(0x401000)));
});
























#include "paging.hpp"
#include "debug.hpp"
#include "cpu.hpp"
#include <cmath>
#include <cstring>

using namespace Paging;

extern "C" PageDirectory _system_page_directory;
extern "C" PageTable _system_default_page_entry;
PageDirectory& Paging::directory = _system_page_directory;
PageTable* const Paging::tables = reinterpret_cast<PageTable*>(GB(4) - MB(4));
PageTable Paging::metatable;

void Paging::init()
{
	log("Page directory address: 0x%X", &directory);
	log("Page tables: 0x%X", tables);

	directory.entries[0].present = 0;

	memset(&metatable, 0, sizeof(PageTable));

	// Enable the metatable.
	PageDirEntry& mt = directory.entries[1023];
	mt.set_address(reinterpret_cast<size_t>(&metatable) - 0xC0000000);
	mt.present = 1;
	mt.writable = 1;

	// Make the metatable self-referencing.
	PageTableEntry& mte = metatable.entries[1023];
	mte.set_address(mt.get_address());
	mte.present = 1;
	mte.writable = 1;

	// Add the startup page table to the metatable.
	PageTableEntry& system_page_entry = metatable.get_entry(reinterpret_cast<void*>(0xC0000000));
	system_page_entry.set_address(reinterpret_cast<size_t>(&_system_default_page_entry) - 0xC0000000);
	system_page_entry.present = 1;
	system_page_entry.writable = 0;

	//CPU::hang();

	log("Has table: %b", directory.has_table(0));
}

size_t Paging::dir_index(void* virt)
{
	return reinterpret_cast<u32>(virt) / MB(4);
}

size_t Paging::tbl_index(void* virt)
{
	return (reinterpret_cast<u32>(virt) % MB(4)) / KB(4);
}

#include "paging.hpp"
#include "debug.hpp"
#include "cpu.hpp"
#include <cmath>
#include <cstring>

using namespace Paging;

extern "C" PageDirectory _system_page_directory;
PageDirectory& Paging::directory = _system_page_directory;
PageTable* const Paging::tables = reinterpret_cast<PageTable*>(GB(4) - MB(4));
PageTable Paging::metatable;

void Paging::init()
{
	log("Page directory address: 0x%X", &directory);
	log("Page tables: 0x%X", tables);

	directory.entries[0].present = 0;

	memset(&metatable, 0, sizeof(PageTable));

	PageDirEntry& mt = directory.entries[1023];
	mt.set_address(reinterpret_cast<size_t>(&metatable) - 0xC0000000);
	mt.present = 1;
	mt.writable = 1;

	CPU::hang();

	log("Has table: %b", directory.has_table(0));
}

size_t Paging::dir_index(size_t virt)
{
	return ceildiv(virt, MB(4));
}

size_t Paging::tbl_index(size_t virt)
{
	return ceildiv(virt % MB(4), KB(4));
}

#include "paging.hpp"
#include "debug.hpp"
#include <cmath>

using namespace Paging;

extern "C" PageDirectory _system_page_directory;
PageDirectory& Paging::directory = _system_page_directory;
PageTable* const Paging::tables = reinterpret_cast<PageTable*>(GB(4) - MB(4));

void Paging::init()
{
	log("Page directory address: 0x%X", &directory);
	log("Page tables: 0x%X", tables);

	directory.get_entry(0).present = 0;
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

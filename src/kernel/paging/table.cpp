#include "paging.hpp"
#include <cmath>

using namespace Paging;

////////////////
// Page Table //
////////////////

PageTableEntry& PageTable::get_entry(size_t virt)
{
	size_t tbl_i = tbl_index(virt);
	return entries[tbl_i];
}


//////////////////////
// Page Table Entry //
//////////////////////

size_t PageTableEntry::get_address()
{
	return address << 12;
}

void PageTableEntry::set_address(size_t phys)
{
	address = phys >> 12;
}

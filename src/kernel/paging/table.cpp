#include "paging.hpp"
#include <cmath>

using namespace Paging;

////////////////
// Page Table //
////////////////

PageTableEntry& PageTable::get_entry(void* virt)
{
	size_t tbl_i = tbl_index(virt);
	return entries[tbl_i];
}

void* PageTable::get_address()
{
	return reinterpret_cast<void*>((reinterpret_cast<u8*>(this) - reinterpret_cast<u8*>(tables)) / KB(4) * MB(4));
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

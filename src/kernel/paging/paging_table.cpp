#include "paging.hpp"
#include "result.hpp"
#include "pmem.hpp"
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

void* PageTableEntry::get_virtual_address()
{
	size_t a = -MB(4);
	size_t b = reinterpret_cast<size_t>(this);
	size_t c = b - a;
	size_t d = sizeof(PageTableEntry);
	size_t e = c / d;
	size_t f = e * KB(4);
	return reinterpret_cast<void*>(f);
}

ResultState PageTableEntry::alloc_any_memory()
{
	Result<void*> result = PMem::alloc_end(KB(4));
	if (result.is_fail())
		return FAIL;

	set_address(reinterpret_cast<size_t>(result.result));
	this->present = 1;
	return SUCCESS;
}

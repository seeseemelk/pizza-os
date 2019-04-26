#include "paging.hpp"
#include "result.hpp"
#include "pmem.hpp"
#include "test.hpp"
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

TEST(PRE_PAGING, "page_table_entry::get_address()", {
	PageTableEntry entry;
	entry.set_address(0x12345000);
	assertEquals("Page entry address not correctly stored", 0x12345000, entry.get_address());
});

TEST(PRE_PAGING, "page_table_entry::get_virtual_address(first_entry)->0", {
	PageTableEntry* entry = reinterpret_cast<PageTableEntry*>(-MB(4));
	assertEquals("Page entry virtual address incorrect", 0, reinterpret_cast<u32>(entry->get_virtual_address()));
});

TEST(PRE_PAGING, "page_table_entry::get_virtual_address(second_entry)->1", {
	PageTableEntry* entry = reinterpret_cast<PageTableEntry*>(-MB(4) + sizeof(PageTableEntry));
	assertEquals("Page entry virtual address incorrect", KB(4), reinterpret_cast<u32>(entry->get_virtual_address()));
});

TEST(PRE_PAGING, "page_table_entry::get_virtual_address(second_table)->1", {
	PageTableEntry* entry = reinterpret_cast<PageTableEntry*>(-MB(4) + sizeof(PageTable));
	assertEquals("Page entry virtual address incorrect", MB(4), reinterpret_cast<u32>(entry->get_virtual_address()));
});














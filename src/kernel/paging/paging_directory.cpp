#include "paging.hpp"

#include "cpu.hpp"
#include "debug.hpp"
#include "libc/math.hpp"
#include "libc/string.hpp"
#include "pmem.hpp"

using namespace Paging;

////////////////////
// Global methods //
////////////////////

PageTable& Paging::get_table(const void* virt)
{
	size_t dir_i = dir_index(virt);
	return tables[dir_i];
}

Result<PageTable*> Paging::get_or_make_table(const void* virt)
{
	if (directory.get_entry(virt).present)
		return Result<PageTable*>(&get_table(virt));
	else
		return directory.get_entry(virt).make_table();
}

////////////////////
// Page Directory //
////////////////////

PageDirEntry& PageDirectory::get_entry(const void* virt)
{
	size_t dir_i = dir_index(virt);
	return entries[dir_i];
}

bool PageDirectory::has_table(const void* virt)
{
	return get_entry(virt).present;
}

//////////////////////////
// Page Directory Entry //
//////////////////////////

size_t PageDirEntry::get_address()
{
	return address << 12;
}

void PageDirEntry::set_address(size_t phys)
{
	address = phys >> 12;
}

PageTable& PageDirEntry::get_table()
{
	size_t block = (reinterpret_cast<size_t>(this) - reinterpret_cast<size_t>(directory.entries)) / sizeof(PageDirEntry);
	log("Block = %d, address = 0x%X", block, reinterpret_cast<size_t>(tables + block));
	return tables[block];
}

Result<PageTable*> PageDirEntry::make_table()
{
	Result<void*> pmem_result = PMem::alloc_end(KB(4));
	if (pmem_result.is_success())
		return Result<PageTable*>(&make_table(reinterpret_cast<size_t>(pmem_result.result)));
	else
		return Result<PageTable*>();
}

PageTable& PageDirEntry::make_table(size_t phys)
{
	size_t block = (reinterpret_cast<size_t>(this) - reinterpret_cast<size_t>(directory.entries)) / sizeof(PageDirEntry);

	// Store the metatable entry.
	PageTableEntry& entry = metatable.entries[block];
	entry.set_address(phys);
	entry.present = 1;
	entry.writable = 1;
	PageTable& table = tables[block];

	// Clear each entry.
	for (size_t i = 0; i < 1024; i++)
		memset(reinterpret_cast<void*>(&table.entries[i]), 0, sizeof(PageTableEntry));

	// Store the directory entry.
	set_address(phys);
	present = 1;
	writable = 1;

	return table;
}

void* PageDirEntry::get_virtual_address()
{
	return reinterpret_cast<void*>((reinterpret_cast<size_t>(this) - reinterpret_cast<size_t>(directory.entries)) / sizeof(PageDirEntry) * MB(4));
}

Result<PageTable*> PageDirEntry::get_or_make_table()
{
	return present ? Result<PageTable*>(&get_table()) : make_table();
}








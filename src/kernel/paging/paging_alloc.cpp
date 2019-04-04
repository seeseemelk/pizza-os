#include "paging.hpp"
#include "debug.hpp"

using namespace Paging;

size_t start_entry = dir_index(reinterpret_cast<void*>(0xC1000000));

PageDirEntry& Paging::alloc_dir_entry()
{
	return directory.entries[start_entry++];
}

Result<PageTable*> Paging::alloc_table(PageDirEntry** entry)
{
	PageDirEntry& dir_entry = alloc_dir_entry();
	*entry = &dir_entry;
	if (dir_entry.present)
		return Result<PageTable*>(&dir_entry.get_table());
	else
		return dir_entry.make_table();
}

Result<PageTable*> Paging::alloc_table()
{
	PageDirEntry* entry;
	return alloc_table(&entry);
}

size_t allocation_table_index = 0;;
PageTable* allocation_table = nullptr;
Result<PageTableEntry*> Paging::alloc_table_entry()
{
	if (allocation_table == nullptr)
	{
		Result<PageTable*> result = alloc_table();
		if (result.is_fail())
		{
			log("Failed to allocate page for page allocations");
			return Result<PageTableEntry*>();
		}
		allocation_table = result.result;
	}

	return Result<PageTableEntry*>(allocation_table->entries[allocation_table_index++]);
}

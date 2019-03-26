#include "paging.hpp"

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

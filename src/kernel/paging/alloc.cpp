#include "paging.hpp"

using namespace Paging;

size_t start_entry = dir_index(reinterpret_cast<void*>(0xC1000000));

PageDirEntry& Paging::alloc_dir_entry()
{
	return directory.entries[start_entry++];
}

Result<PageTable*> Paging::alloc_table()
{
	PageDirEntry& entry = alloc_dir_entry();
	if (entry.present)
		return Result<PageTable*>(&entry.get_table());
	else
		return entry.make_table();
}

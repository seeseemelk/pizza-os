#include "paging.hpp"
#include <cmath>

using namespace Paging;

PageDirEntry& PageDirectory::get_entry(size_t virt)
{
	size_t dir_i = dir_index(virt);
	return directory.entries[dir_i];
}

bool PageDirectory::has_table(size_t virt)
{
	return get_entry(virt).present;
}

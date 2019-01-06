#include "paging.hpp"
#include <cmath>

using namespace Paging;

////////////////////
// Page Directory //
////////////////////

PageDirEntry& PageDirectory::get_entry(size_t virt)
{
	size_t dir_i = dir_index(virt);
	return directory.entries[dir_i];
}

bool PageDirectory::has_table(size_t virt)
{
	return get_entry(virt).present;
}

PageTable& PageDirectory::get_table(size_t virt)
{
	size_t dir_i = dir_index(virt);
	return tables[dir_i];
}


//////////////////////////
// Page Directory Entry //
//////////////////////////

size_t PageDirEntry::get_address()
{
	return address << 12;
}

void PageDirEntry::set_address(size_t virt)
{
	address = virt >> 12;
}

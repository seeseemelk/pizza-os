#include "paging.hpp"
#include "pmem.hpp"
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

void PageDirEntry::set_address(size_t phys)
{
	address = phys >> 12;
}

Result PageDirEntry::make_table()
{
	Result result;
	PMem::Result pmem_result = PMem::alloc_end(KB(4));
	if (pmem_result.state == PMem::Result::SUCCESS)
	{
		result.state = Result::SUCCESS;
		result.table = &make_table(reinterpret_cast<size_t>(pmem_result.address));;
	}
	else
		result.state = Result::FAIL;
	return result;
}

PageTable& PageDirEntry::make_table(size_t phys)
{
	size_t block = (this - directory.entries) / sizeof(PageDirEntry);
	/*set_address(phys);
	present = 1;
	writable = 1;*/

	// Store the metatable entry.
	PageTableEntry& entry = metatable.entries[block];
	entry.set_address(phys);
	entry.present = 1;
	entry.writable = 1;
	PageTable& table = tables[block];

	// Clear each entry.
	for (size_t i = 0; i < 1024; i++)
	{
		table.entries[i].present = 0;
	}

	// Store the directory entry.
	set_address(phys);
	present = 1;
	writable = 1;

	return table;
}












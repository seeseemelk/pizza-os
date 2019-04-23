#include "process.hpp"
#include "paging.hpp"
#include "pmem.hpp"

using namespace Proc;

ResultState Process::map_page(void* address)
{
	Paging::PageDirEntry& entry = Paging::directory.get_entry(address);
	Result<Paging::PageTable*> result = entry.get_or_make_table();
	if (result.is_fail())
		return ResultState::FAIL;
	Paging::PageTable* table = result.result;
	Paging::PageTableEntry& pageEntry = table->get_entry(address);

	entry.userspace = 1;
	entry.present = 1;
	entry.writable = 1;

	if (pageEntry.present)
		return ResultState::SUCCESS;

	Result<void*> memResult = PMem::alloc_end(KB(4));
	if (memResult.is_fail())
		return ResultState::FAIL;

	pageEntry.set_address(reinterpret_cast<u32>(memResult.result));
	pageEntry.writable = 1;
	pageEntry.userspace = 1;
	pageEntry.present = 1;
	return ResultState::SUCCESS;
}

ResultState Process::map_pages(void* address, size_t blocks)
{
	for (size_t i = 0; i < blocks; i++)
	{
		if (map_page(address) == ResultState::FAIL)
			return ResultState::FAIL;
		address = static_cast<u8*>(address) + KB(4);
	}
	return ResultState::SUCCESS;
}






















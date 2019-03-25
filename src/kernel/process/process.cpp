#include "process.hpp"
#include "paging.hpp"
#include "pmem.hpp"

using namespace Processes;

ResultState Process::map_page(void* address)
{
	auto entry = Paging::directory.get_entry(address);
	auto result = entry.get_or_make_table();
	if (result.is_fail())
		return ResultState::FAIL;
	auto table = result.result;
	auto pageEntry = table->get_entry(address);

	if (pageEntry.present)
		return ResultState::SUCCESS;

	auto memResult = PMem::alloc_end(KB(4));
	if (memResult.is_fail())
		return ResultState::FAIL;

	pageEntry.set_address(reinterpret_cast<u32>(memResult.result));
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

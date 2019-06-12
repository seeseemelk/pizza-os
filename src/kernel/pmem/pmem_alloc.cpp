#include "pmem.hpp"
#include "cpu.hpp"
#include "debug.hpp"
#include "test.hpp"
#include <cmath>

using namespace PMem;

Result<void*> PMem::alloc_end(size_t bytes)
{
	size_t blocks = ceildiv(bytes, KB(4));
	size_t block_end = map_length;

	for (size_t i = map_length - 1; i != static_cast<size_t>(-1); i--)
	{
		if (static_cast<BlockState>(map[i]) != FREE)
			block_end = i;
		else if (block_end - i == blocks)
		{
			size_t address = i * KB(4);
			size_t length = blocks * KB(4);
			set_state(address, length, BlockState::USED);
			return Result<void*>( reinterpret_cast<void*>(address) );
		}
	}

	log("Fail alloc_end");
	return Result<void*>();
}

ResultState PMem::increase_ref_count(size_t addr)
{
	if (map[addr / KB(4)] < BlockState::RESERVED - 1)
	{
		map[addr / KB(4)]++;
		return ResultState::SUCCESS;
	}
	else
		return ResultState::FAIL;
}

void PMem::free(size_t addr)
{
	if (map[addr / KB(4)] > BlockState::FREE)
		map[addr / KB(4)]--;
}

TEST(POST_PMEM, "PMem::increase_ref_count/free() -> ref_count increased and decreased", {
	size_t addr = KB(4)*4;
	assertEquals("Ref count not correctly initialised", 0, PMem::get_state(addr));
	assertEquals("Ref count not increased", ResultState::SUCCESS, PMem::increase_ref_count(addr));
	assertEquals("Ref count not correctly set", 1, PMem::get_state(addr));
	PMem::free(addr);
	assertEquals("Ref count not correctly freed", 0, PMem::get_state(addr));
});

TEST(POST_PMEM, "PMem::alloc_end() -> state is 1", {
	Result<void*> result = PMem::alloc_end(1);
	assertTrue("Could not allocate", result.is_success());
	assertEquals("Incorrect block state", 1, PMem::get_state(reinterpret_cast<size_t>(result.result)));
	PMem::free(result.result);
});

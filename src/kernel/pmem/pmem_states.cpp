#include "pmem.hpp"
#include "debug.hpp"
#include "cpu.hpp"
#include "test.hpp"
#include <cmath>

using namespace PMem;

void PMem::set_state(size_t start_addr, size_t bytes_length, BlockState state)
{
	size_t start_block = start_addr / KB(4);
	size_t stop_block = start_block + ceildiv(bytes_length, KB(4));

	if (stop_block > PMem::map_length || stop_block < start_block)
	{
		log("PMem::set_state out-of-bounds.");
		CPU::hang();
	}

	for (size_t i = start_block; i < stop_block; i++)
	{
		map[i] = static_cast<u8>(state);
	}
}

BlockState PMem::get_state(size_t addr)
{
	size_t block = addr / KB(4);
	return static_cast<BlockState>(map[block]);
}

TEST(POST_PMEM, "PMem::get_state(known_block) -> unused", {
	assertEquals("Block state is not unused", BlockState::FREE, PMem::get_state(KB(4)*4));
});

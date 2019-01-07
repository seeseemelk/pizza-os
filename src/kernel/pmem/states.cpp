#include "pmem.hpp"
#include "debug.hpp"
#include <cmath>

using namespace PMem;

void PMem::set_state(size_t start_addr, size_t bytes_length, BlockState state)
{
	size_t start_block = start_addr / KB(4);
	size_t stop_block = start_block + (bytes_length / KB(4));

	for (size_t i = start_block; i < stop_block; i++)
	{
		map[i] = state;
	}
}

BlockState PMem::get_state(size_t addr)
{
	size_t block = addr / KB(4);
	return static_cast<BlockState>(map[block]);
}

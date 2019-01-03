#include "pmem.hpp"
#include "debug.hpp"
#include <cmath>

void PMem::set_state(size_t start_addr, size_t bytes_length, BlockState state)
{
	size_t start_block = ceildiv(start_addr, KB(4));
	size_t stop_block = start_block + (bytes_length / KB(4));

	for (size_t i = start_block; i < stop_block; i++)
	{
		map[i] = state;
	}
}

#include "pmem.hpp"
#include <cmath>

using namespace PMem;

Result<void*> PMem::alloc_start(size_t bytes)
{
	size_t blocks = ceildiv(bytes, KB(4));
	size_t block_start = 0;

	for (size_t i = 0; i < map_length; i++)
	{
		if (static_cast<BlockState>(map[i]) != FREE)
			block_start = i;
		else if (i - block_start > blocks)
			return Result<void*>(reinterpret_cast<void*>((block_start + 1) * KB(4)));
	}

	return Result<void*>();
}

Result<void*> PMem::alloc_end(size_t bytes)
{
	size_t blocks = ceildiv(bytes, KB(4));
	size_t block_end = 0;

	for (size_t i = map_length - 1; i != static_cast<size_t>(-1); i--)
	{
		if (static_cast<BlockState>(map[i]) != FREE)
			block_end = i;
		else if (block_end - i > blocks)
			return Result<void*>(reinterpret_cast<void*>(i * KB(4)));
	}

	return Result<void*>();
}

#include "pmem.hpp"
#include <cmath>

using namespace PMem;

#define GET_BLOCK(block) static_cast<BlockState>(map[block])

Result PMem::alloc_start(size_t bytes)
{
	Result result;
	size_t blocks = ceildiv(bytes, KB(4));
	size_t block_start = 0;

	for (size_t i = 0; i < map_length; i++)
	{
		if (GET_BLOCK(i) != FREE)
			block_start = i;
		else if (i - block_start > blocks)
			goto success;
	}

	//fail:
	result.state = Result::FAIL;
	return result;

	success:
	result.state = Result::SUCCESS;
	result.address = reinterpret_cast<void*>((block_start + 1) * KB(4));
	return result;
}

Result PMem::alloc_end(size_t bytes)
{
	Result result;
	size_t blocks = ceildiv(bytes, KB(4));
	size_t block_end = 0;

	size_t i = map_length - 1;
	for (; i != static_cast<size_t>(-1); i--)
	{
		if (GET_BLOCK(i) != FREE)
			block_end = i;
		else if (block_end - i > blocks)
			goto success;
	}

	//fail:
	result.state = Result::FAIL;
	return result;

	success:
	result.state = Result::SUCCESS;
	result.address = reinterpret_cast<void*>(i * KB(4));
	return result;
}

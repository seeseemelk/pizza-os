#include "pmem.hpp"
#include "cpu.hpp"
#include "debug.hpp"
#include <cmath>

using namespace PMem;

/* This contains bugs but also isn't used.
Result<void*> PMem::alloc_start(size_t bytes)
{
	log("Enter alloc_start");
	size_t blocks = ceildiv(bytes, KB(4));
	size_t block_start = 0;

	for (size_t i = 0; i < map_length; i++)
	{
		if (static_cast<BlockState>(map[i]) != FREE)
			block_start = i;
		else if (i - block_start > blocks)
			return Result<void*>(reinterpret_cast<void*>((block_start + 1) * KB(4)));
	}

	log("Leave alloc_start");
	return Result<void*>();
}
*/

Result<void*> PMem::alloc_end(size_t bytes)
{
	size_t blocks = ceildiv(bytes, KB(4));
	size_t block_end = map_length;

	//log("Allocating %d continuous blocks of physical memory", blocks);

	for (size_t i = map_length - 1; i != static_cast<size_t>(-1); i--)
	{
		if (static_cast<BlockState>(map[i]) != FREE)
			block_end = i;
		else if (block_end - i == blocks)
		{
			size_t address = i * KB(4);
			size_t length = blocks * KB(4);
			set_state(address, length, BlockState::USED);
			//log("Allocated at 0x%X", address);
			return Result<void*>( reinterpret_cast<void*>(address) );
		}
	}

	log("Fail alloc_end");
	return Result<void*>();
}

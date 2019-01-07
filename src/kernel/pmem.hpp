#ifndef PMEM_HPP_
#define PMEM_HPP_

#include <cstddef>
#include <cstdint>

namespace PMem
{
	extern u8* map;
	extern size_t map_length;

	enum BlockState
	{
		FREE, USED, RESERVED, NON_EXISTANT
	};

	struct Result
	{
		enum {SUCCESS, FAIL} state;
		void* address;
		//enum BlockState block_state;
	};

	void init();
	BlockState get_state(size_t addr);
	void set_state(size_t start_addr, size_t bytes_length, BlockState state);

	Result alloc_end(size_t bytes);
	Result alloc_start(size_t bytes);
}

#endif

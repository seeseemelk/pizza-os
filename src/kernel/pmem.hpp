#ifndef PMEM_HPP_
#define PMEM_HPP_

#include <cstddef>
#include <cstdint>
#include "result.hpp"

namespace PMem
{
	extern u8 map[];
	extern size_t map_length;

	enum BlockState
	{
		FREE, USED, RESERVED, NON_EXISTANT
	};

	void init();
	BlockState get_state(size_t addr);
	void set_state(size_t start_addr, size_t bytes_length, BlockState state);

	Result<void*> alloc_end(size_t bytes);
	Result<void*> alloc_start(size_t bytes);
}

#endif

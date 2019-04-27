#ifndef PMEM_HPP_
#define PMEM_HPP_

#include <cstddef>
#include <cstdint>
#include "result.hpp"

namespace PMem
{
	extern u8* map;
	extern size_t map_length;

	enum BlockState
	{
		FREE = 0x00,
		USED,
		RESERVED = 0xFF,
		NON_EXISTANT = 0xFE
	};

	void init();
	BlockState get_state(size_t addr);
	void set_state(size_t start_addr, size_t bytes_length, BlockState state);

	ResultState increase_ref_count(size_t addr);
	void free(size_t addr);
	inline void free(void* addr)
	{
		free(reinterpret_cast<size_t>(addr));
	}

	Result<void*> alloc_end(size_t bytes);
	Result<void*> alloc_start(size_t bytes);
}

#endif

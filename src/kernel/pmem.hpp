#ifndef PMEM_HPP_
#define PMEM_HPP_

#include <cstddef>
#include <cstdint>

namespace PMem
{
	extern u8* map;

	enum BlockState
	{
		FREE, USED, RESERVED, NON_EXISTANT
	};

	struct Result
	{
		enum {SUCCESS, FAIL} result;
		enum BlockState block_state;
	};

	void init();
	Result get_state(size_t addr);
	void set_state(size_t start_addr, size_t bytes_length, BlockState state);
}

#endif

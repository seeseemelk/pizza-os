#ifndef PMEM_HPP_
#define PMEM_HPP_

#include <cstddef>

namespace PMem
{

	enum BlockState
	{
		FREE, USED, RESERVED, NON_EXISTANT
	};

	void init();
	BlockState get_state(size_t block);
	void set_state(size_t blocks, BlockState state);
}

#endif

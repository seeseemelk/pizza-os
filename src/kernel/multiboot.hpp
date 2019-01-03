#ifndef MULTIBOOT_HPP_
#define MULTIBOOT_HPP_

extern "C"
{
#include "multiboot.h"
}
#include <cstddef>

namespace Multiboot
{
	extern multiboot_info_t* mbt;

	//size_t memory_size();
}

#endif

#ifndef MULTIBOOT_HPP_
#define MULTIBOOT_HPP_

#include "libc/stddef.hpp"

extern "C"
{
#include "multiboot.h"
}

namespace Multiboot
{
	extern multiboot_info_t* mbt;

	//size_t memory_size();
}

#endif

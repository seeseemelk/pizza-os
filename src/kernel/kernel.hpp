#ifndef KERNEL_HPP_
#define KERNEL_HPP_

#include "libc/stddef.hpp"
#include "libc/stdint.hpp"
#include "result.hpp"

namespace Kernel
{
	const u32 kernel_start = GB(3U);
	Result<void*> alloc(size_t bytes);
}

#endif

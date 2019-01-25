#ifndef KERNEL_HPP_
#define KERNEL_HPP_

#include "result.hpp"
#include <cstdint>
#include <cstddef>

namespace Kernel
{
	const u32 kernel_start = GB(3U);
	Result<void*> alloc(size_t bytes);
}

#endif

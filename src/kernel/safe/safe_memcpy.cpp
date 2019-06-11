#include "safe.hpp"
#include <cstdint>

using namespace Safe;

SafeResult Safe::memcpy(char* dest, const char* src, size_t bytes)
{
	if (!exists(dest))
		return SafeResult::PAGE_NOT_PRESENT;
	if (!exists(src))
		return SafeResult::PAGE_NOT_PRESENT;

	int read_protection = protection_level(dest);
	int write_protection = protection_level(src);

	size_t read_bytes_to_recheck = KB(4) - (reinterpret_cast<u32>(src) % KB(4));
	size_t write_bytes_to_recheck = KB(4) - (reinterpret_cast<u32>(dest) % KB(4));

	while (bytes-- > 0)
	{
		if (read_bytes_to_recheck-- == 0)
		{
			if (!exists(src))
				return SafeResult::PAGE_NOT_PRESENT;
			if (protection_level(src) != read_protection)
				return SafeResult::PROTECTION_BOUNDS_CROSSED;
			read_bytes_to_recheck = KB(4);
		}

		if (write_bytes_to_recheck-- == 0)
		{
			if (!exists(dest))
				return SafeResult::PAGE_NOT_PRESENT;
			if (protection_level(dest) != write_protection)
				return SafeResult::PROTECTION_BOUNDS_CROSSED;
			write_bytes_to_recheck = KB(4);
		}

		*(dest++) = *(src++);
	}

	return SafeResult::OK;
}

#include "safe.hpp"
#include "test.hpp"
#include <cstdint>

using namespace Safe;

SafeResult Safe::strnlen(size_t& result, const char* src, size_t limit)
{
	log("A");
	if (!exists(src))
		return SafeResult::PAGE_NOT_PRESENT;
	log("AA");
	int protection = protection_level(src);
	size_t bytes_until_recheck = KB(4) - (reinterpret_cast<u32>(src) % KB(4));

	log("B");

	result = 0;
	while (limit-- > 0)
	{
		if (*(src++) == '\0')
			return SafeResult::OK;

		if (bytes_until_recheck-- == 0)
		{
			if (!exists(src))
				return SafeResult::PAGE_NOT_PRESENT;
			if (protection_level(src) != protection)
				return SafeResult::PROTECTION_BOUNDS_CROSSED;
			bytes_until_recheck = KB(4);
		}

		result++;
	}

	log("C");

	return SafeResult::OK;
}

/*TEST(OTHER, "Safe::strnlen normal", {
	const char* str = "hello, world!";
	size_t result;
	assertEquals("strnlen failed", SafeResult::OK, Safe::strnlen(result, str, 128));
	assertEquals("strnlen returned an invalid result", 13, result);
});*/

#include "safe.hpp"

using namespace Safe;

SafeResult Safe::strncpy(char* dest, const char* src, size_t limit)
{
	SafeResult result;
	size_t copy_limit;
	if ((result = strnlen(copy_limit, src, limit)) != SafeResult::OK)
		return result;

	return memcpy(dest, src, copy_limit);
}

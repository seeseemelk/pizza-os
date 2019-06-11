#include "safe.hpp"

using namespace Safe;

const char* Safe::describe(SafeResult result)
{
	switch (result)
	{
		case SafeResult::OK:
			return "operation was successful";
		case SafeResult::PAGE_NOT_PRESENT:
			return "page was not present during operation";
		case SafeResult::PROTECTION_BOUNDS_CROSSED:
			return "memory protection boundaries were crossed during operation";
		default:
			return "unknown result";
	}
}

/*
 * This file contains utility functions such as strncpy that
 * check user/kernel space boundaries.
 */
#ifndef SAFE_HPP_
#define SAFE_HPP_

#include "libc/stddef.hpp"

namespace Safe
{
	enum [[nodiscard]] SafeResult
	{
		OK,
		PROTECTION_BOUNDS_CROSSED,
		PAGE_NOT_PRESENT
	};

	bool exists(const void* addr);
	int protection_level(const void* addr);

	// Copies memory safely.
	SafeResult memcpy(char* dest, const char* src, size_t bytes);

	// Checks the length of a string safely.
	// The result will be the length of the string minus the 0-terminator,
	// but it will never be higher than limit.
	SafeResult strnlen(size_t& result, const char* src, size_t limit);

	// Copies a string safely.
	// It will copy at most limit bytes.
	// The last character it writes will always be a null terminator.
	SafeResult strncpy(char* dest, const char* src, size_t limit);

	// Describes a result.
	const char* describe(SafeResult result);
}

#endif

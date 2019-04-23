#ifndef DEBUG_HPP_
#define DEBUG_HPP_

extern "C" {
#include <stdarg.h>
}

namespace Debug
{
	void init();
	void init_serial();

	void put(const char c);
	void puts(const char* str);
	void printf(const char* fmt, ...);
}

#define log(fmt, ...) \
	::Debug::printf("[%s:%d] " fmt "\n", __FILE__, __LINE__, ## __VA_ARGS__)

#define UNUSED(x) \
	(void)(x)

#endif

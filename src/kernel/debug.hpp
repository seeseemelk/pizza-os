#ifndef DEBUG_HPP_
#define DEBUG_HPP_

extern "C" {
#include <stdarg.h>
}

#ifndef MODULE
#define MODULE "UNKNOWN"
#endif

namespace Debug
{
	void init();
	void init_serial();

	void put(const char c);
	void puts(const char* str);
	void printf(const char* fmt, ...);
}

#define log(fmt, ...) \
	Debug::printf("[%s:%d] " fmt "\n", __FILE__, __LINE__, ## __VA_ARGS__)

/*
#define log(fmt) \
	Debug::printf("[%s:%d] " fmt, __FILE__, __LINE__)
*/

#endif

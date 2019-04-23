#include "syscall.hpp"
#include "debug.hpp"

u32 Syscall::Debug::print(u32 fmt, u32 a, u32 b)
{
	char* fmt_str = reinterpret_cast<char*>(fmt);
	log("LOG: %s (0x%X, 0x%X)", fmt_str, a, b);
	return 0;
}

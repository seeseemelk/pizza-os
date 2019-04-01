#ifndef CPU_HPP_
#define CPU_HPP_

#include <cstdint>

namespace CPU
{
	void init();
	void init_gdt();
	void halt();
	[[noreturn]] void hang();
	[[noreturn]] void out_of_memory();

	u32 get_cr2();
	void load_cr3(u32 phys_address);
	void to_usermode(u32 entrypoint);
}

#endif

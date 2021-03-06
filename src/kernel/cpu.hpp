#ifndef CPU_HPP_
#define CPU_HPP_

#include "libc/stdint.hpp"

namespace CPU
{
	void init();
	void init_gdt();
	void halt();
	[[noreturn]] void hang();
	[[noreturn]] void out_of_memory();
	[[noreturn]] void reset();

	u32 get_cr2();
	void load_cr3(u32 phys_address);
	void set_ring3_syscall_stack(void* stack);
}

#endif

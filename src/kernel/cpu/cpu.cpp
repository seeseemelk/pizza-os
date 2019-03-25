#include "cpu.hpp"
#include "interrupt.hpp"
#include "debug.hpp"

using namespace CPU;

void CPU::init()
{
	init_gdt();
}

void CPU::halt()
{
	asm("hlt");
}

[[noreturn]] void CPU::hang()
{
	Interrupt::disable();
	log("Hanging");
	while (true)
		halt();
}

[[noreturn]] void CPU::out_of_memory()
{
	log("Out of memory");
	hang();
}

extern "C" u32 asm_get_cr2();
u32 CPU::get_cr2()
{
	return asm_get_cr2();
}

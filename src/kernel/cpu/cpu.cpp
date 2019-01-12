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

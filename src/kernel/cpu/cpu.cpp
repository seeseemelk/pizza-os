#include "cpu.hpp"

#include "debug.hpp"
#include "interrupt.hpp"
#include "io.hpp"
#include "test.hpp"

using namespace CPU;

void CPU::init()
{
	init_gdt();
}

void CPU::halt()
{
	asm("hlt");
}

#ifndef TESTING
[[noreturn]] void CPU::hang()
{
	Interrupt::disable();
	log("Hanging");
	while (true)
		halt();
}
#else
[[noreturn]] void CPU::hang()
{
	Test::Asserts::fail("Kernel hanging");
}
#endif

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

extern "C" u32 asm_load_cr3(u32);
void CPU::load_cr3(u32 address)
{
	asm_load_cr3(address);
}

IO::Port keyboardController(0x64);
[[noreturn]] void CPU::reset()
{
	Interrupt::disable();
	while (1)
	{
		keyboardController.outb(0xFE);
	}
}

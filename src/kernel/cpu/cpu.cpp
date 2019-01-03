#include "cpu.hpp"
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

void CPU::hang()
{
	log("Hanging");
	while (true)
		halt();
}

void CPU::disable_interrupts()
{
	asm ("cli");
}

void CPU::enable_interrupts()
{
	asm ("sti");
}

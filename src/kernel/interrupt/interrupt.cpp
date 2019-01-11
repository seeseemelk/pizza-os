#include "interrupt.hpp"
#include "debug.hpp"

using namespace Interrupt;

IDT Interrupt::idt;
IDTR Interrupt::idtr;

extern "C" void handle_interrupt(int irq, int error_code)
{
	log("IRQ: 0x%X, error: 0x%X", irq, error_code);
}

static void connect_interrupt(int irq)
{
	Gate gate = idt.gates[irq];
	gate.set_type(GateType::INTERRUPT);
	gate.dpl = 0;
}

void Interrupt::init()
{
	for (size_t i = 0; i < 0x20; i++)
		connect_interrupt(i);
}

void Interrupt::disable()
{
	asm ("cli");
}

void Interrupt::enable()
{
	asm ("sti");
}

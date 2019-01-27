#include "interrupt.hpp"
#include "debug.hpp"
#include "slab.hpp"
#include "paging.hpp"
#include "handler.hpp"
#include "cpu.hpp"
#include "paging.hpp"
#include "result.hpp"
#include "kernel.hpp"
#include <cstring>

#define MAX(x, y) ((x >= y) ? (x) : (y))

using namespace Interrupt;

extern char INTH;
extern char INTH_END;
extern char INTH_ERR;
extern char INTH_ERR_END;

IDT* Interrupt::idt;
IDTR Interrupt::idtr;

HandlerFactory inth_factory;
HandlerFactory inth_err_factory;

Slab<char*> handlers;

extern "C" void handle_interrupt(int irq, int error_code)
{
	log("IRQ: 0x%X, error: 0x%X", irq, error_code);
}

static char* create_handler(size_t irq, HandlerFactory& factory)
{
	// Allocate memory to store the interrupt handler.
	Result<char**> result = handlers.alloc();
	if (result.is_fail())
		CPU::out_of_memory();

	// Build the handler
	char* dest = reinterpret_cast<char*>(result.result);
	log("Building interrupt handler at 0x%X", dest);
	factory.build(dest, irq);

	// Setup the handler in the IDT.
	Gate gate = idt->gates[irq];
	gate.present = 1;
	gate.set_offset(dest);
	gate.set_type(GateType::INTERRUPT);

	return dest;
}

static void create_idt()
{
	log("Allocing");
	Result<void*> result = Kernel::alloc(sizeof(IDT));
	log("Done");

	if (result.is_fail())
		CPU::out_of_memory();

	idt = static_cast<IDT*>(result.result);

	log("Memsetting");
	memset(static_cast<void*>(idt), 0, sizeof(IDT));
	log("Also done");
}

static void setup_idtr()
{
	idtr.address = reinterpret_cast<u32>(idt);
	idtr.limit = sizeof(IDT) - 1;
	asm("lidt %0" :: "m" (idtr));
}

void Interrupt::init()
{
	inth_factory.load(&INTH, &INTH_END);
	inth_err_factory.load(&INTH_ERR, &INTH_ERR_END);

	Result<Paging::PageTable*> result = Paging::alloc_table();
	if (!result.is_success())
		CPU::out_of_memory();

	size_t inth_size = inth_factory.length();
	size_t inth_err_size = inth_err_factory.length();
	handlers = Slab<char*>(*result.result, static_cast<size_t>( MAX(inth_size, inth_err_size) ));

	log("Setting up IDT");
	create_idt();
	log("Setting up IDTR");
	setup_idtr();
	log("Creating some handlers");

	create_handler(0x0, inth_factory);
	create_handler(0x1, inth_factory);
}

void Interrupt::disable()
{
	asm ("cli");
}

void Interrupt::enable()
{
	asm ("sti");
}

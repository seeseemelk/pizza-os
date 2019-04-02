#include "interrupt.hpp"
#include "debug.hpp"
#include "slab.hpp"
#include "paging.hpp"
#include "cpu.hpp"
#include "paging.hpp"
#include "result.hpp"
#include "kernel.hpp"
#include "interrupt/handler_factory.hpp"
#include "interrupt/handler.hpp"
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
	Gate& gate = idt->gates[irq];
	gate.present = 1;
	gate.dpl = 3;
	gate.segment = 8;
	gate.size = 1;
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
	handlers.init(*result.result, static_cast<size_t>( MAX(inth_size, inth_err_size) ));

	log("Setting up IDT");
	create_idt();
	log("Setting up IDTR");
	setup_idtr();
	log("Creating some handlers");

	static const unsigned int without_errors[] =
			{0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06,
			 0x07, 0x09, 0x10, 0x12, 0x13, 0x14};

	static const unsigned int with_errors[] =
			{0x08, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x11,
			 0x0F, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A,
			 0x1B, 0x1C, 0x1D, 0x1E, 0x1F};

	for (size_t i = 0; i < sizeof(without_errors) / sizeof(int); i++)
		create_handler(without_errors[i], inth_factory);

	for (size_t i = 0; i < sizeof(with_errors) / sizeof(int); i++)
		create_handler(with_errors[i], inth_err_factory);

	// Create handler for external devices
	for (size_t i = 0x20; i <= 0x30; i++)
		create_handler(i, inth_factory);

	// Before enabling, the PIC interrupt vectors have to be remapped.
	PIC::init();
}

void Interrupt::disable()
{
	asm ("cli");
}

void Interrupt::enable()
{
	asm ("sti");
}

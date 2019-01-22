#include "interrupt.hpp"
#include "debug.hpp"
#include "slab.hpp"
#include "paging.hpp"
#include "handler.hpp"
#include "cpu.hpp"
#include "paging.hpp"
#include "result.hpp"

#define MAX(x, y) ((x >= y) ? (x) : (y))

using namespace Interrupt;

extern char INTH;
extern char INTH_END;
extern char INTH_ERR;
extern char INTH_ERR_END;

IDT Interrupt::idt;
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
	Result<char**> result = handlers.alloc();
	if (result.is_fail())
		CPU::out_of_memory();

	char* dest = reinterpret_cast<char*>(result.result);
	log("Building interrupt handler at 0x%X", dest);
	factory.build(dest, irq);
	return dest;
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

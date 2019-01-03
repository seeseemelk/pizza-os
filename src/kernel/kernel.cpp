#include <cstddef>
#include "debug.hpp"
#include "cpu.hpp"
#include "pmem.hpp"

extern "C" void _init(void);
extern "C" void _fini(void);

extern "C" void kernel_main()
{
	_init();

	Debug::puts("Initialising CPU...");
	CPU::init();
	Debug::puts(" DONE!\n");

	Debug::puts("Initialising PMEM allocator...");
	PMem::init();
	Debug::puts(" DONE!\n");

	CPU::hang();
	_fini();
}

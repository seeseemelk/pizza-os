#include <cstddef>
#include "debug.hpp"
#include "cpu.hpp"
#include "pmem.hpp"
#include "multiboot.hpp"

extern "C" void _init(void);
extern "C" void _fini(void);

extern "C" void kernel_main(multiboot_info_t* mbt)
{
	Multiboot::mbt = mbt;
	_init();

	log("Initialising CPU...");
	CPU::init();
	log("Done");

	//Multiboot::memory_size();

	log("Initialising PMEM allocator...");
	PMem::init();
	log("Done");

	CPU::hang();
	_fini();
}

#include <cstddef>
#include "debug.hpp"
#include "cpu.hpp"
#include "pmem.hpp"
#include "paging.hpp"
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

	log("Initialising Paging...");
	Paging::init();
	log("Done");

	Paging::directory.get_entry(0x12345678).make_table();

	CPU::hang();
	_fini();
}

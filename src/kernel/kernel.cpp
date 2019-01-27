#include <cstddef>
#include "debug.hpp"
#include "cpu.hpp"
#include "pmem.hpp"
#include "paging.hpp"
#include "multiboot.hpp"
#include "interrupt.hpp"
#include "slab.hpp"
#include "ramdisk.hpp"
#include "kernel/alloc.hpp"

extern "C" void _init(void);
extern "C" void _fini(void);

extern "C" void kernel_main(multiboot_info_t* mbt)
{
	Multiboot::mbt = mbt;
	_init();
	Debug::init();

	log("Initialising CPU...");
	CPU::init();
	log("Done");

	//Multiboot::memory_size();

	log("Initialising PMEM allocator...");
	PMem::init();
	log("Done");

	log("Initialising paging...");
	Paging::init();
	log("Done");

	log("Initialising Kernel::alloc");
	Kernel::init_alloc();
	log("Done");

	log("Initialising interrupts...");
	Interrupt::init();
	log("Done");

	log("Loading ramdisk...");
	RamDisk::init();
	log("Done");

	log("Kernel main ended");
	CPU::hang();
	_fini();
}

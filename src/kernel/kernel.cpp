#include "cpu.hpp"
#include "debug.hpp"
#include "interrupt.hpp"
#include "kernel/alloc.hpp"
#include "multiboot.h"
#include "multiboot.hpp"
#include "paging.hpp"
#include "pmem.hpp"
#include "process.hpp"
#include "ramdisk.hpp"
#include "result.hpp"
#include "scheduler.hpp"
#include "vfs.hpp"

extern "C" void _init(void);
extern "C" void _fini(void);
extern "C" void prog_test(void);

extern "C" void kernel_main(multiboot_info_t* mbt)
{
	Multiboot::mbt = mbt;
	_init();
	Debug::init();

	#ifdef TEST
		log("Running tests");
		CPU::reset();
	#else
		log("Initialising CPU...");
		CPU::init();
		log("Done");

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

		log("Initialising process management...");
		Proc::init();
		log("Done");

		log("Initialising VFS...");
		VFS::init();
		log("Done");

		log("Loading ramdisk...");
		RamDisk::init();
		log("Done");

		log("Loading mcp...");
		Result<Proc::Process*> result = Proc::exec_new_initrd("mcp");
		if (result.is_fail())
		{
			log("Failed to load mcp");
			CPU::hang();
		}

		log("Passing control to scheduler");
		Scheduler::run();

		log("Kernel main ended");
		CPU::hang();
		_fini();
	#endif
}

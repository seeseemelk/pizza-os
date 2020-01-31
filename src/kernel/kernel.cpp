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
#include "syscall.hpp"
#include "test.hpp"

extern "C" void _init(void);
extern "C" void _fini(void);
extern "C" void prog_test(void);

extern "C" void kernel_main(multiboot_info_t* mbt)
{
	Multiboot::mbt = mbt;
	_init();
	Debug::init();

	#ifdef TESTING
		Test::begin_tests();
		Test::execute_tests(Test::Group::PRE_CPU);
	#endif
	log("Initialising CPU...");
	CPU::init();
	log("Done");

	#ifdef TESTING
		Test::execute_tests(Test::Group::PRE_PMEM);
	#endif
	log("Initialising PMEM allocator...");
	PMem::init();
	log("Done");

	#ifdef TESTING
		Test::execute_tests(Test::Group::PRE_PAGING);
	#endif
	log("Initialising paging...");
	Paging::init();
	log("Done");

	#ifdef TESTING
		Test::execute_tests(Test::Group::PRE_ALLOC);
	#endif
	log("Initialising Kernel::alloc");
	Kernel::init_alloc();
	log("Done");

	#ifdef TESTING
		Test::execute_tests(Test::Group::PRE_INT);
	#endif
	log("Initialising interrupts...");
	Interrupt::init();
	log("Done");

	#ifdef TESTING
		Test::execute_tests(Test::Group::PRE_PROC);
	#endif
	log("Initialising process management...");
	Proc::init();
	log("Done");

	#ifdef TESTING
		Test::execute_tests(Test::Group::PRE_VFS);
	#endif
	log("Initialising VFS...");
	//VFS::init();
	log("Done");

	log("Initialising syscalls...");
	//Syscall::initialise();
	log("Done");

	#ifdef TESTING
		Test::execute_tests(Test::Group::PRE_RAMDISK);
	#endif
	log("Loading ramdisk...");
	RamDisk::init();
	log("Done");

	#ifdef TESTING
		Test::execute_tests(Test::Group::OTHER);
		Test::end_tests();
		CPU::reset();
	#endif
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
}

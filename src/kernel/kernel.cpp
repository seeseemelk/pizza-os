#include <cstddef>
#include "debug.hpp"
#include "cpu.hpp"
#include "pmem.hpp"
#include "paging.hpp"
#include "multiboot.hpp"
#include "interrupt.hpp"
#include "slab.hpp"
#include "ramdisk.hpp"
#include "elf.hpp"
#include "process.hpp"
#include "kernel/alloc.hpp"

extern "C" void _init(void);
extern "C" void _fini(void);
extern "C" void prog_test(void);

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

	log("Initialising process management...");
	Processes::init();
	log("Done");

	log("Loading ramdisk...");
	RamDisk::init();
	log("Done");

	auto result = RamDisk::get_file("mcp");
	if (result.is_fail())
	{
		log("Could not find mcp in ramdisk");
		CPU::hang();
	}

	log("Loading mcp...");
	auto file = result.result;
	TarReader reader(file);
	Elf elf_reader(reader);
	elf_reader.dump();
	if (!elf_reader.is_valid())
	{
		log("Found invalid ELF file");
		CPU::hang();
	}

	log("Found valid ELF file, loading it!");
	if (elf_reader.extract() == ResultState::FAIL)
	{
		log("Failed to load ELF file!");
		CPU::hang();
	}

	log("Finished");

	log("Kernel main ended");
	CPU::hang();
	_fini();
}

#include "ramdisk.hpp"
#include "debug.hpp"
#include "multiboot.hpp"
#include "cpu.hpp"
#include <cstdint>

using namespace RamDisk;

void RamDisk::init()
{
	size_t mods_count = Multiboot::mbt->mods_count;
	size_t mods_addr = Multiboot::mbt->mods_addr;

	if (mods_count != 1)
	{
		log("The kernel needs exactly 1 module, namely the initrd.tar file");
		CPU::hang();
	}
	log("Have %d module(s) starting at 0x%X", mods_count, mods_addr);

	multiboot_mod_list* module = reinterpret_cast<multiboot_mod_list*>(mods_addr + GB(3));
	const void* initrd_addr = reinterpret_cast<const void*>(module->mod_start + GB(3));
	size_t initrd_length = module->mod_end - module->mod_start;
	log("Module starts at 0x%X and is %d bytes long", initrd_addr, initrd_length);
	init_ramdisk(initrd_addr, initrd_length);
}

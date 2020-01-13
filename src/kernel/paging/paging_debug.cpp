#include "paging.hpp"
#include "debug.hpp"

using namespace Paging;

void Paging::debug_dump(PageDirEntry& entry)
{
	size_t virt_addr = reinterpret_cast<size_t>(entry.get_virtual_address());
	log("PD @ 0x%X -> 0x%X - 0x%X [present: %b, userspace: %b, writable: %b]",
			entry.get_address(),
			virt_addr, virt_addr + 0x3FFFFF,
			entry.present, entry.userspace, entry.writable);
	for (int i = 0; i < 1024; i++)
		debug_dump(entry.get_table().entries[i]);
}

void Paging::debug_dump(PageTableEntry& entry)
{
	size_t virt_addr = reinterpret_cast<size_t>(entry.get_virtual_address());
	log("PT @ 0x%X -> 0x%X - 0x%X [present: %b, userspace: %b, writable: %b]",
			entry.get_address(),
			virt_addr, virt_addr + 0xFFF,
			entry.present, entry.userspace, entry.writable);
}

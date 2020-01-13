#include "paging.hpp"
#include "debug.hpp"

using namespace Paging;

void Paging::debug_dump(PageDirEntry& entry)
{
	size_t virt_addr = reinterpret_cast<size_t>(entry.get_virtual_address());
	log("PD @ 0x%X -> 0x%X - 0x%X [ptable_phys: 0x%X, present: %b, userspace: %b, writable: %b]",
			reinterpret_cast<size_t>(&entry),
			virt_addr, virt_addr + 0x3FFFFF,
			entry.get_address(),
			entry.present, entry.userspace, entry.writable);
	if (entry.present)
		debug_dump(entry.get_table());
}

void Paging::debug_dump(PageTable& table)
{
	log("Table: 0x%X", reinterpret_cast<size_t>(&table));
	for (int i = 0; i < 1024; i++)
	{
		if (table.entries[i].present)
			debug_dump(table.entries[i]);
	}
}

void Paging::debug_dump(PageTableEntry& entry)
{
	size_t virt_addr = reinterpret_cast<size_t>(entry.get_virtual_address());
	log("PT @ 0x%X -> 0x%X - 0x%X [phys: 0x%X, present: %b, userspace: %b, writable: %b]",
			reinterpret_cast<size_t>(&entry),
			virt_addr, virt_addr + 0xFFF,
			entry.get_address(),
			entry.present, entry.userspace, entry.writable);
}

void Paging::debug_dump()
{
	debug_dump_metatable();
	log("=== Dumping entire page table ===");
	for (int i = 0; i < 1024; i++)
	{
		if (directory.entries[i].present)
		{
			log("Entry %d is present", i);
			debug_dump(directory.entries[i]);
		}
	}
}

void Paging::debug_dump_metatable()
{
	log("=== Dumping metatable ===");
	for (int i = 0; i < 1024; i++)
	{
		if (metatable.entries[i].present)
		{
			debug_dump(metatable.entries[i]);
		}
	}
}

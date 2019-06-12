#include "safe.hpp"
#include "paging.hpp"
#include "debug.hpp"
#include "test.hpp"

using namespace Safe;

bool Safe::exists(const void* addr)
{
	log("Address: 0x%X", addr);
	log("Dir_index: 0x%X", Paging::dir_index(addr));
	log("table_index: 0x%X", Paging::tbl_index(addr));
	log("Address: 0x%X", (GB(4) - MB(4)) + Paging::dir_index(addr) * KB(4) + Paging::tbl_index(addr) * 4);
	log("A");
	while (1);
	Paging::PageDirEntry& dir = Paging::directory.get_entry(addr);
	if (!dir.present)
		return false;
	log("Dir is: 0x%X", dir.present);
	Paging::PageTableEntry& table = dir.get_table().get_entry(addr);
	log("Table entry: 0x%X", &table);
	log("Table contents: 0x%X", table);
	if (!table.present)
		return false;
	log("C");
	return true;
}

int Safe::protection_level(const void* addr)
{
	u32 addr32 = reinterpret_cast<u32>(addr);
	if (addr32 >= GB(3))
		return 0;
	else
		return 1;
}

TEST(OTHER, "Safe::exists(kernel_memory) -> true", {
	assertEquals("Safe::exists failed", true, Safe::exists(reinterpret_cast<void*>(&Safe::exists)));
});

TEST(OTHER, "Safe::exists(non_existant_memory) -> false", {
	assertEquals("Safe::exists failed", false, Safe::exists(nullptr));
});

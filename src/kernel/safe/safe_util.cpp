#include "safe.hpp"
#include "paging.hpp"
#include "debug.hpp"
#include "test.hpp"

using namespace Safe;

bool Safe::exists(const void* addr)
{
	Paging::PageDirEntry& dir = Paging::directory.get_entry(addr);
	if (!dir.present)
		return false;
	Paging::PageTableEntry& table = dir.get_table().get_entry(addr);
	if (!table.present)
		return false;
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

TEST(OTHER, "Safe::exists(non_existant_memory) -> false", {
	assertEquals("Safe::exists failed", false, Safe::exists(nullptr));
})

TEST(OTHER, "Safe::exists(kernel_memory) -> true", {
	assertEquals("Safe::exists failed", true, Safe::exists(reinterpret_cast<void*>(0xC0000000)));
})

#include "safe.hpp"
#include "paging.hpp"
#include "debug.hpp"

using namespace Safe;

bool Safe::exists(const void* addr)
{
	log("A");
	Paging::PageDirEntry& dir = Paging::directory.get_entry(addr);
	if (!dir.present)
		return false;
	log("B");
	Paging::PageTableEntry& table = dir.get_table().get_entry(addr);
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

TEST(OTHER, "exist", {
	const char* str = "hello, world!";
	size_t result;
	assertEquals("strnlen failed", SafeResult::OK, Safe::strnlen(result, str, 128));
	assertEquals("strnlen returned an invalid result", 13, result);
});

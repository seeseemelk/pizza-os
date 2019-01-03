#ifndef PAGING_HPP_
#define PAGING_HPP_

#include <cstddef>
#include <cstdint>

namespace Paging
{

struct PageDirEntry
{
	u32 present : 1;
	u32 writable : 1;
	u32 userspace : 1;
	u32 pwt : 1; // Page-level write-through: ???
	u32 pcd : 1; // Page-level cache disable: ???
	u32 accessed : 1;
	u32 IGNORE1 : 1;
	u32 large_page : 1; // 1 if it maps a 4-MiB page, 0 if it references a page table.
	u32 IGNORE2 : 4;
	u32 address : 20;
};

struct PageTableEntry
{
	u32 present : 1;
	u32 writable : 1;
	u32 userspace : 1;
	u32 pwt : 1; /* Page-level write-through: ??? */
	u32 pcd : 1; /* Page-level cache disable: ??? */
	u32 accessed : 1;
	u32 dirty : 1;
	u32 pat : 1; /* ???, but only supported since PIII */
	u32 global : 1; /* ??? */
	u32 IGNORE1 : 3;
	u32 address : 20;

	void set_address(size_t address);
};

struct PageTable
{
	PageTableEntry entries[1024];
};

struct PageDirectory
{
	PageDirEntry entries[1024];

	bool has_table(size_t virt);
	PageTable get_table(size_t virt);
};

extern PageDirectory& directory;

void init();

}

#endif

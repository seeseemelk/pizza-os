#ifndef PAGING_HPP_
#define PAGING_HPP_

#include "result.hpp"
#include <cstddef>
#include <cstdint>

namespace Paging
{

struct PageTable;

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

	void set_address(size_t phys);
	size_t get_address();
	Result<PageTable*> make_table();
	PageTable& make_table(size_t phys);
	PageTable& get_table();
	Result<PageTable*> get_or_make_table();
	void* get_virtual_address();
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

	void set_address(size_t phys);
	size_t get_address();
	void* get_virtual_address();
	ResultState alloc_any_memory();
};

struct PageTable
{
	PageTableEntry entries[1024];

	PageTableEntry& get_entry(const void* virt);
	void* get_address();
} __attribute__((aligned(4096)));

struct PageDirectory
{
	PageDirEntry entries[1024];

	PageDirEntry& get_entry(const void* virt);
	bool has_table(const void* virt);
	PageTable& get_table(const void* virt);
	Result<PageTable*> get_or_make_table(const void* virt);
} __attribute__((aligned(4096)));

extern PageDirectory& directory;
extern PageTable* const tables;
extern PageTable metatable;

void init();
size_t dir_index(const void* virt);
size_t tbl_index(const void* virt);
PageDirEntry& alloc_dir_entry();
Result<PageTable*> alloc_table(PageDirEntry** directory_entry);
Result<PageTable*> alloc_table();
Result<PageTableEntry*> alloc_table_entry();
void load_directory(u32 directory);
void debug_dump(PageDirEntry& entry);
void debug_dump(PageTableEntry& entry);
//void debug_dump();

}

#endif


















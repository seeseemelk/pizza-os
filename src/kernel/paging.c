#include "paging.h"
#include "pmem.h"

void page_init()
{
	arch_page_init();
}

void page_allocate(virt_addr_t mem_start, size_t pages)
{
	for (size_t i = 0; i < pages; i++)
	{
		void* phys = pmem_alloc(KB(4));
		page_assign(mem_start + i*4096, phys);
	}
}

void page_assign(virt_addr_t page, phys_addr_t phys_addr)
{
	page_assign_many(page, phys_addr, 1);
}

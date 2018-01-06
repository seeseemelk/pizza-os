/*
 * mem.c
 *
 *  Created on: Jan 3, 2018
 *      Author: seeseemelk
 */
#include "mem.h"
#include "cdefs.h"
#include "kernel.h"
#include "page.h"

#include <string.h>
#include <stdio.h>
#include <stdbool.h>

#define PAGE_SIZE KB(4) - sizeof(size_t)

typedef struct mem_page mem_page;
struct mem_page
{
	size_t size;
	u8 data[PAGE_SIZE];
};

mem_page* mem_base;
size_t mem_length;

bool paging_enabled = false;

static void mem_set_region(size_t address, size_t size)
{
	mem_length = size / sizeof(mem_page);
	mem_base = (mem_page*) address;
	printf("Memory base at 0x%p and is %d pages long\n", mem_base, mem_length);
	printf("Size of pages = %d bytes\n", sizeof(mem_page));
}

static size_t mem_bytes_to_pages(size_t bytes)
{
	if (bytes > 0 && bytes <= PAGE_SIZE)
		return 1;
	else
	{
		int pages = 1;
		bytes -= PAGE_SIZE;
		pages += bytes / sizeof(mem_page);
		return pages;
	}
}

static mem_page* mem_get_page(void* ptr)
{
	return (mem_page*) (ptr - sizeof(size_t));
}

void* mem_alloc(size_t size)
{
	// First, how many pages do we need to allocate?
	size_t pages_needed = mem_bytes_to_pages(size);

	// Go through the map to see if we find that many free pages.
	mem_page* first_page = NULL;
	size_t pages_found = 0;

	size_t index = 0;
	while (index < mem_length && pages_found < pages_needed)
	{
		mem_page* page = &mem_base[index];
		if (page->size == 0)
		{
			if (pages_found == 0)
				first_page = page;
			pages_found++;
		}
		else
			pages_found = 0;
		index += mem_bytes_to_pages(page->size);
	}

	if (first_page == NULL)
	{
		return NULL;
	}
	else
	{
		first_page->size = size;
		return first_page->data;
	}
}

void mem_free(void* address)
{
	mem_get_page(address)->size = 0;
}

void* mem_realloc(void* address, size_t new_size)
{
	mem_page* page = mem_get_page(address);
	size_t old_pages = mem_bytes_to_pages(page->size);
	size_t new_pages = mem_bytes_to_pages(new_size);

	if (new_pages == old_pages)
	{
		page->size = new_size;
		return address;
	}
	else if (new_pages > old_pages)
	{
		void* new_addr = mem_alloc(new_size);
		memcpy(new_addr, address, page->size);
		return new_addr;
	}
	else
	{
		for (size_t i = new_pages; i < old_pages; i++)
			mem_base[i].size = 0;
		page->size = new_size;
		return address;
	}
}

void mem_init(size_t address, size_t size)
{
	mem_set_region(address, size);
	for (size_t i = 0; i < mem_length; i++)
	{
		mem_base[i].size = 0;
	}
}

void mem_paging_enabled()
{
	for (int i = 0; i < mem_length; i++)
	{
		mem_page* page = &mem_base[i];
		page_alloc_over(page, page);
		/*if (page->size > 0)
		{*/
		//index += mem_bytes_to_pages(page->size);
		/*}
		else
			index++;*/
	}
}






















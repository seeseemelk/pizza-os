/*
 * page.c
 *
 *  Created on: Jan 3, 2018
 *      Author: seeseemelk
 */
#include "arch/pc/page.h"
#include "arch/pc/asmfn.h"
#include "page.h"
#include "kernel.h"
#include "cdefs.h"

#include <stdlib.h>
#include <stdio.h>

page_entry* directory;

size_t page_get_address(page_entry* entry)
{
	return (*entry) & 0xFFFFF000;
}

void page_set_address(page_entry* entry, void* address)
{
	(*entry) = ((*entry) & 0xFFF) | (((size_t) address) & 0xFFFFF000);
}

void page_set_flags(page_entry* entry, bool present, bool rw, bool user)
{
	(*entry) = page_get_address(entry) | ((present) | (rw << 1) | (user << 2));
}

static page_entry* page_get_dir_entry(size_t entry)
{
	return &directory[entry];
}

static page_entry* page_get_tbl_entry(page_entry* dir, size_t entry)
{
	page_entry* tbl = (page_entry*) page_get_address(dir);
	return &tbl[entry];
}

static page_entry* page_get_virt(void* virt_addr)
{
	page_entry* dir_entry = page_get_dir_entry(((size_t) virt_addr) / MB(4));
	return page_get_tbl_entry(dir_entry, ((size_t) virt_addr) % MB(4));
}

page* page_alloc()
{
	kernel_panic("Not implemented");
	return NULL;
}

/**
 * Allocates a single page so that the page contains
 * the specified physical address.
 */
page* page_alloc_phys(void* phys_addr)
{
	kernel_panic("Not implemented");
		return NULL;
}

/**
 * Allocates a single page so that the page contains
 * the specified virtual address.
 */
page* page_alloc_virt(void* virt_addr)
{
	kernel_panic("Not implemented");
	return NULL;
}

/**
 * Allocates a single page so that the specified virtual address
 * accesses the specified physical address.
 */
page* page_alloc_over(void* phys_addr, void* virt_addr)
{
	page_entry* entry = page_get_virt(virt_addr);
	page_set_address(entry, phys_addr);
	page_set_flags(entry, true, true, false);
	return (page*) entry;
}

/**
 * Frees a certain page.
 */
void page_free(page* p)
{
	page_entry* entry = (page_entry*) p;
	page_set_flags(entry, false, false, false);
}

/*
 * Paging functions
 */
void page_init()
{
	printf("Initialising paging\n");

	/*
	 * Generate the initial page directory.
	 */
	directory = calloc(1024, sizeof(page_entry));
	if (directory == 0)
		kernel_panic("Could not allocate page directory");
	page_alloc_over(directory, directory);
}

void page_enable()
{
	printf("Enabling paging\n");

	/*
	 * Load the address of the page directory into CR3
	 */
	asm_load_cr3_page_dir((size_t) directory);

	/*
	 * Set CR0.PG to 1.
	 * This will put the CPU in 32-bit paging mode.
	 * (see Intel Manual Volume 3, 4.1.2)
	 */
	asm_enable_cr0_pepg();

	printf("Paging enabled\n");
}




















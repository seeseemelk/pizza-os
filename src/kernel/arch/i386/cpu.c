/*
 * cpu.c
 *
 *  Created on: Jan 3, 2018
 *      Author: seeseemelk
 */
#include "../i386/cpu.h"

#include "arch/i386/asmfn.h"
#include "cdefs.h"
#include "pmem.h"
#include "kernel.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

/*
 * GDT structs
 */

typedef struct gdt_entry gdt_entry;
typedef struct gdt_descriptor gdt_descriptor;

struct __attribute__((packed)) gdt_entry
{
	u16 limit0_15;
	u16 base0_15;
	u8 base16_23;
	u8 access;
	u8 limit16_19; /* Also contains flags */
	u8 base24_31;
};

struct __attribute__((packed)) gdt_descriptor
{
	u16 limit;
	u32 address;
};

gdt_entry gdt_table[3];
gdt_descriptor descriptor;

/*
 * GDT functions
 */
static void gdt_set_limit(gdt_entry* entry, u32 limit)
{
	entry->limit0_15 = limit & 0xFFFF;
	entry->limit16_19 = (entry->limit16_19 & 0xF0) | ((limit >> 16) & 0xF);
}

static void gdt_set_base(gdt_entry* entry, u32 base)
{
	entry->base0_15 = base & 0xFFFF;
	entry->base16_23 = (base >> 16) & 0xFF;
	entry->base24_31 = (base >> 24) & 0xFF;
}

static void gdt_set_access(gdt_entry* entry, bool present, int privilege, bool executable, bool dc, bool rw)
{
	// 0x10 -> set reserved bit to 1 and set accessed bit to 0
	entry->access = 0x10 | (present << 7) | (privilege << 5) | (executable << 3) | (dc << 2) | (rw << 1);
}

static void gdt_set_flags(gdt_entry* entry, bool granularity)
{
	// 0x40 -> set size bit to 1 (32-bit protected mode)
	entry->limit16_19 = (entry->limit16_19 & 0x0F) | (granularity << 7 | 0x40);
}

static void gdt_zero_entry(gdt_entry* entry)
{
	entry->limit0_15 = 0;
	entry->base0_15 = 0;
	entry->base16_23 = 0;
	entry->access = 0;
	entry->limit16_19 = 0;
	entry->base24_31 = 0;
}

void gdt_load(gdt_descriptor* descriptor)
{
	asm __volatile__ (
			"lgdt %0;"
			:
			: "m" (*descriptor));
	asm_reload_segment_registers();
}

/*
 * General cpu functions
 */
void cpu_init()
{
	gdt_zero_entry(&gdt_table[0]);

	gdt_set_base(gdt_table+1, 0);
	gdt_set_limit(gdt_table+1, 0xFFFFFFFF);
	gdt_set_flags(gdt_table+1, 1);
	gdt_set_access(gdt_table+1, true, 0, true, false, true);

	gdt_set_base(gdt_table+2, 0);
	gdt_set_limit(gdt_table+2, 0xFFFFFFFF);
	gdt_set_flags(gdt_table+2, 1);
	gdt_set_access(gdt_table+2, true, 0, false, false, true);

	descriptor.address = (int) gdt_table;
	descriptor.limit = (((int) (gdt_table+3)) - 1) - ((int) gdt_table);
	gdt_load(&descriptor);
}


















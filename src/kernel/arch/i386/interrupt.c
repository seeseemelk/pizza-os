/*
 * interrupt.c
 *
 *  Created on: Jan 24, 2018
 *      Author: seeseemelk
 */

#include "arch/i386/interrupt.h"

#include "cdefs.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>

#define BIT_PRESENT (1<<15)

typedef struct idt_descriptor idt_descriptor;
struct idt_descriptor
{
	u8 flags;
	u32 offset;
	u8 segment;
} __attribute__((packed));

idt_descriptor* idt;

void idt_set_present(idt_descriptor* descriptor, bool present)
{
	descriptor->flags = (descriptor->flags & ~BIT_PRESENT) & (BIT_PRESENT * present);
}

void lidt(idt_descriptor* idt)
{
	u64 val = 256;
	val |= ((size_t)idt << 16);
	asm("lidt %0"
			:
			: "m" (val));
}

static void init_idt()
{
	idt = malloc(sizeof(idt_descriptor) * 256);

	for (int i = 0; i < 256; i++)
		idt_set_present(idt+i, false);

	lidt(idt);
}

void arch_interrupt_init()
{
	init_idt();
}

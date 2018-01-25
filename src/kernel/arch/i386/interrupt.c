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
#include <stdio.h>

extern void* inth_start;
extern void* inth_end;

#define GATE_MASK		0b00001111

#define T_INT_GATE 		0b00001110
#define T_TRAP_GATE 	0b00001111
#define T_TASK_GATE		0b00000101
#define BIT_PRESENT		0b10000000

#define INT_HANDLER_SIZE ((size_t)&inth_end - (size_t)&inth_start)

typedef struct int_descriptor int_descriptor;

struct int_descriptor
{
	u16 offset1;
	u16 segment;
	u8 zero;
	u8 flags;
	u16 offset2;
} __attribute__((packed));

int_descriptor* idt;
char* handlers;

static void idt_set_type(int_descriptor* descriptor, u16 type)
{
	descriptor->flags = (descriptor->flags & ~GATE_MASK) | type;
}

static void idt_set_address(int_descriptor* descriptor, void* addr, u16 segment)
{
	descriptor->offset1 = ((u32)addr) & 0xFFFF;
	descriptor->offset2 = (((u32)addr) >> 16) & 0xFFFF;
	descriptor->segment = segment;
}

static void idt_set_present(int_descriptor* descriptor, bool present)
{
	descriptor->flags = (descriptor->flags & ~BIT_PRESENT) | (BIT_PRESENT * present);
}

void lidt(int_descriptor* idt)
{
	u64 val = 256;
	val |= ((size_t)idt << 16);
	asm volatile("lidt %0"
			:
			: "m" (val));
}

static void init_idt()
{
	idt = malloc(sizeof(int_descriptor) * 256);

	for (int i = 0; i < 256; i++)
	{
		idt[i].flags = 0;
		idt_set_type(idt+i, T_INT_GATE);
		idt_set_present(idt+i, true);
		idt_set_address(idt+i, (void*)((size_t)handlers+INT_HANDLER_SIZE*i), 0);
	}

	lidt(idt);
}

static void init_handlers()
{
	char* original_handler = (char*)&inth_start;

	printf("\n");
	printf("Begin: 0x%p\n", &inth_start);
	printf("End: 0x%p\n", &inth_end);
	printf("Size: %d\n", INT_HANDLER_SIZE);

	handlers = malloc(INT_HANDLER_SIZE * 256);
	for (int i = 0; i < 256; i++)
	{
		char* handler = (char*)((size_t)handlers + i*INT_HANDLER_SIZE);
		for (int b = 0; b < INT_HANDLER_SIZE; b++)
		{
			handler[b] = original_handler[b];
		}
	}
}

void arch_interrupt_init()
{
	init_handlers();
	init_idt();
	//while(1);
	while(1);
	asm volatile("sti");

	while(1);
	//printf("Something %d", *((char*)0xFF0000));
}

void cpu_int_handler(int interrupt_n)
{
	printf("Interrupt");
	while(1);
}

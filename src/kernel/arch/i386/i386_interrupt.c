/*
 * interrupt.c
 *
 *  Created on: Jan 24, 2018
 *      Author: seeseemelk
 */

#include <interrupt.h>
#include "i386_asm.h"
#include "i386_interrupt.h"
#include "i386_pic.h"
#include "cdefs.h"
#include "kernel.h"
#include "page.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>

extern void* inth_start;
extern void* inth_end;

#define GATE_MASK		0b00011111

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
	descriptor->offset1 = (u16) (((u32)addr) & 0xFFFF);
	descriptor->offset2 = (u16) ((((u32)addr) >> 16) & 0xFFFF);
	descriptor->segment = segment;
}

static void idt_set_present(int_descriptor* descriptor, bool present)
{
	if (!present)
		descriptor->flags &= ~BIT_PRESENT;
	else
		descriptor->flags |= BIT_PRESENT;
}

static void lidt(int_descriptor* idt)
{
	u64 val = 256 * sizeof(int_descriptor) - 1;
	val |= ((u64)((size_t)idt)) << 16;
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
		idt[i].zero = 0;
		idt_set_type(idt+i, T_INT_GATE);
		idt_set_present(idt+i, true);
		idt_set_address(idt+i, (void*)(handlers+INT_HANDLER_SIZE*i), 8);
	}

	lidt(idt);
}

static void init_handlers()
{
	char* original_handler = (char*)&inth_start;

	handlers = malloc(INT_HANDLER_SIZE * 256);
	for (int i = 0; i < 256; i++)
	{
		char* handler = (char*)((size_t)handlers + i*INT_HANDLER_SIZE);
		for (unsigned int b = 0; b < INT_HANDLER_SIZE; b++)
		{
			handler[b] = original_handler[b];
		}
		*((u32*)(handler+2)) = i;
	}
}

void arch_interrupt_init()
{
	cli();
	init_handlers();
	init_idt();
	pic_init();
	sti();
}

void cpu_int_handler(int irq, int error_code)
{
	if (irq == 0x27)
	{
		if (!pic_is_served(irq))
		{
			kernel_log("Spurious interrupt on master");
			return;
		}
	}
	else if (irq == 0x2F)
	{
		if (!pic_is_served(irq))
		{
			// This will still send an EOI to the master PIC, but not the slave.
			pic_send_eoi(7);
			kernel_log("Spurious interrupt on slave");
			return;
		}
	}

	interrupt_handle(irq, error_code);
}

void arch_interrupt_finish(irq_t* irq)
{
	if (irq->num >= 0x20)
	{
		pic_send_eoi(irq->num);
	}
}

void interrupt_enable()
{
	sti();
}

void interrupt_disable()
{
	cli();
}

void interrupt_debug_jmp(int exceptionNumber, void* target)
{
	/*u8* opcode = (u8*) (handlers + INT_HANDLER_SIZE * exceptionNumber);*/

	/* MOV EAX, target */
	/*opcode[0] = 0xB8;
	*((u32*) (opcode+1)) = (size_t) target;
	opcode += 5;*/

	/* JMP EAX */
	/*opcode[0] = 0x66;
	opcode[1] = 0xFF;
	opcode[2] = 0xE0;*/

	int_descriptor* entry = idt + exceptionNumber;
	idt_set_address(entry, target, 8);
}







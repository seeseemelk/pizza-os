/*
 * io.c
 *
 *  Created on: Jan 2, 2018
 *      Author: seeseemelk
 */
#include "../../io.h"

void outb(u16 port, u8 value)
{
	asm volatile("outb %0, %1"
		:
		: "a" (value), "d" (port));
}

void outw(u16 port, u16 value)
{
	asm volatile("outw %0, %1"
			:
			: "a" (value), "d" (port));
}

void outl(u16 port, u32 value)
{
	asm volatile("outl %0, %1"
			:
			: "a" (value), "d" (port));
}

u8 inb(u16 port)
{
	u8 value;
	asm volatile("inb %1, %0"
			: "=a" (value)
			: "d" (port));
	return value;
}

u16 inw(u16 port)
{
	u16 value;
	asm volatile("inw %1, %0"
			: "=a" (value)
			: "d" (port));
	return value;
}

u32 inl(u16 port)
{
	u32 value;
	asm volatile("inl %1, %0"
			: "=a" (value)
			: "d" (port));
	return value;
}

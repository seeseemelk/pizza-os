/*
 * pic.c
 *
 *  Created on: Jan 25, 2018
 *      Author: seeseemelk
 */

#include "i386_pic.h"
#include "io.h"
#include "cdefs.h"
#include <stdbool.h>

#define MSR_CMD 0x0020
#define MSR_DAT 0x0021
#define SLV_CMD  0x00A0
#define SLV_DAT  0X00A1

#define PIC_EOI 0x20
#define PIC_OCW3 0x0A

void pic_send_eoi(u8 irq)
{
	if (irq >= 0x28)
		outb(SLV_CMD, PIC_EOI);
	outb(MSR_CMD, PIC_EOI);
}

bool pic_is_served(u8 irq)
{
	if (irq <= 0x27)
	{
		outb(MSR_CMD, PIC_OCW3);
		u8 served = inb(MSR_CMD);
		return (served & (1 << (irq - 0x20))) > 0;
	}
	else
	{
		outb(SLV_CMD, PIC_OCW3);
		u8 served = inb(SLV_CMD);
		return (served & (1 << (irq - 0x28))) > 0;
	}
}

void pic_init()
{
	outb(MSR_CMD, 0x11);
	outb(SLV_CMD, 0x11);
	outb(MSR_DAT, 0x20);
	outb(SLV_DAT, 0x28);
	outb(MSR_DAT, 0x04);
	outb(SLV_DAT, 0x02);

	outb(MSR_DAT, 0x01);
	outb(SLV_DAT, 0x01);
}

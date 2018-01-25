/*
 * pic.c
 *
 *  Created on: Jan 25, 2018
 *      Author: seeseemelk
 */

#include "arch/i386/pic.h"

#include "io.h"

#define MSR_CMD 0x0020
#define MSR_DAT 0x0021
#define SLV_CMD  0x00A0
#define SLV_DAT  0X00A1

#define PIC_EOI 0x20

void pic_send_eoi(u8 irq)
{
	if (irq >= 0x28)
		outb(SLV_CMD, PIC_EOI);
	outb(MSR_CMD, PIC_EOI);
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

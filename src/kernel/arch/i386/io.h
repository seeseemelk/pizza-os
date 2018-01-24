/*
 * io.h
 *
 *  Created on: Jan 2, 2018
 *      Author: seeseemelk
 */

#ifndef ARCH_I386_IO_H_
#define ARCH_I386_IO_H_
#include "cdefs.h"

void outb(u16 port, u8 value);
void outw(u16 port, u16 value);
void outl(u16 port, u32 value);

u8 inb(u16 port);
u16 inw(u16 port);
u32 inl(u16 port);

#endif /* ARCH_I386_IO_H_ */

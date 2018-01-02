/*
 * io.h
 *
 *  Created on: Jan 2, 2018
 *      Author: seeseemelk
 */

#ifndef ARCH_PC_IO_H_
#define ARCH_PC_IO_H_
#include <stdint.h>

void outb(int port, uint8_t value);
void outw(int port, uint16_t value);
void outq(int port, uint32_t value);
void outl(int port, uint64_t value);

#endif /* ARCH_PC_IO_H_ */

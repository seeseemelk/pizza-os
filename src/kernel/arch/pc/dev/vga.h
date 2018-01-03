/*
 * vga.h
 *
 *  Created on: Jan 2, 2018
 *      Author: seeseemelk
 */

#ifndef ARCH_PC_DEV_VGA_H_
#define ARCH_PC_DEV_VGA_H_

#include "dev/devices.h"

/*char vga_get_char(const int i);
void vga_set_char(const int i, char c);
void vga_set_cursor(const int i);
int vga_get_cursor();
void vga_scroll(lines);*/

device* vga_init();

#endif /* ARCH_PC_DEV_VGA_H_ */

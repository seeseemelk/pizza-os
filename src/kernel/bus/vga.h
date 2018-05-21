/*
 * vga.h
 *
 *  Created on: May 7, 2018
 *      Author: seeseemelk
 */

#ifndef BUS_VGA_H_
#define BUS_VGA_H_
#include "devices.h"

typedef struct
{
	device_t* dev;
	void(*get_size)(device_t* vga, int* width, int* height);
	char(*get_char)(device_t* vga, int x, int y);
	void(*set_char)(device_t* vga, int x, int y, char c);
	void(*get_cursor)(device_t* vga, int* x, int* y);
	void(*set_cursor)(device_t* vga, int x, int y);
	void(*scroll)(device_t* vga, int lines);
} vga_bus_t;

void vga_get_size(vga_bus_t* vga, int* width, int* height);
char vga_get_char(vga_bus_t* vga, int x, int y);
void vga_set_char(vga_bus_t* vga, int x, int y, char c);
void vga_get_cursor(vga_bus_t* vga, int* x, int* y);
void vga_set_cursor(vga_bus_t* vga, int x, int y);
void vga_scroll(vga_bus_t* vga, int lines);

#endif /* BUS_VGA_H_ */

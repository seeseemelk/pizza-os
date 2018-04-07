/*
 * vga.c
 *
 *  Created on: Jan 2, 2018
 *      Author: seeseemelk
 */
#include "../../i386/dev/vga.h"

#include <string.h>
#include "io.h"

#define R_ADDR 0x3B4
#define R_DATA 0x3B5

module* vga_module;
device* vga_device;
int width = 80;
int height = 25;
char* vga_memory;
int cursor_x = 0;
int cursor_y = 0;

static void vga_write_cursor_address()
{
	int cursor_index = cursor_x + cursor_y * width;
	outb(R_ADDR, 0x0E); // Write the high eight bits of the cursor location
	outb(R_DATA, (cursor_index >> 8) & 0xFF);
	outb(R_ADDR, 0x0F); // Write the low eight bits of the cursor location
	outb(R_DATA, cursor_index & 0xFF);
}

int vga_request(module_request* request)
{
	//GET_CHAR, SET_CHAR, GET_WIDTH, GET_HEIGHT, GET_CURSOR_X, GET_CURSOR_Y, SET_CURSOR, SCROLL
	switch (request->type)
	{
	case GET_WIDTH:
		return width;
	case GET_HEIGHT:
		return height;
	case GET_CHAR:
		return vga_memory[(request->arg1 + request->arg2 * width) * 2];
	case SET_CHAR:
		vga_memory[(request->arg1 + request->arg2 * width) * 2] = request->arg3;
		return 0;
	case GET_CURSOR_X:
		return cursor_x;
	case GET_CURSOR_Y:
		return cursor_y;
	case SET_CURSOR:
		cursor_x = request->arg1;
		cursor_y = request->arg2;
		vga_write_cursor_address();
		return 0;
	case SCROLL:
		//memcpy(vga_memory, vga_memory + (width * request->arg1)*2, width * (height-request->arg1) *2);
		if (request->arg1 > 0)
		{
			int lines = request->arg1;
			memcpy(vga_memory, vga_memory + (width * 2 * lines), width * (height-lines) * 2);
			for (int i = 0; i < width * lines; i++) // We can't use memset because it will mess with the colours
				vga_memory[(width * height - i - lines ) * 2] = vga_memory[(width * height - i) * 2];
			cursor_y -= lines;
			vga_write_cursor_address();
		}
		return 0;
	default:
		return 0;
	}
}

device* vga_init()
{
	// Set the CRT controller address to 0x03B4 and 0x03B5
	outb(0x3C2, (inb(0x3CC) & 0b11111110) | 0b00000010);

	vga_module = module_register("vga", TERMINAL, &vga_request);
	vga_device = device_register(vga_module);
	vga_memory = device_mmap((void*) 0xB8000, KB(8));
	return vga_device;
}





















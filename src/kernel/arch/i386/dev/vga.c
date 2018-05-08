/*
 * vga.c
 *
 *  Created on: Jan 2, 2018
 *      Author: seeseemelk
 */
#include "interrupt.h"
#include "vga.h"
#include "io.h"
#include "bus.h"
#include "bus/vga.h"

#include <string.h>
#include <stdio.h>

#define R_ADDR 0x3B4
#define R_DATA 0x3B5

#define DEV(vga) ((device_t*)vga)
#define VGA(dev) ((vga_t*)dev)

typedef struct
{
	device_t dev;
	int width;
	int height;
	char* memory;
	int cursor_x;
	int cursor_y;
} vga_t;

module_t vga_mod;
vga_t vga_dev = { .width = 80, .height = 25, .cursor_x = 0, .cursor_y = 0 };

static void vga_write_cursor_address(vga_t* vga)
{
	int cursor_index = vga->cursor_x + vga->cursor_y * vga->width;
	outb(R_ADDR, 0x0E); // Write the high eight bits of the cursor location
	outb(R_DATA, (cursor_index >> 8) & 0xFF);
	outb(R_ADDR, 0x0F); // Write the low eight bits of the cursor location
	outb(R_DATA, cursor_index & 0xFF);
}
/*
 int vga_dev_request(dev_req_t* request)
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
 memcpy(vga_memory, vga_memory + (width * 2 * lines),
 width * (height - lines) * 2);
 for (int i = 0; i < width * lines; i++) // We can't use memset because it will mess with the colours
 vga_memory[(width * height - i - lines) * 2] = vga_memory[(width
 * height - i) * 2];
 cursor_y -= lines;
 vga_write_cursor_address();
 }
 return 0;
 default:
 return 0;
 }
 }*/

void dvga_get_size(device_t* dev, int* x, int* y)
{
	*x = VGA(dev)->width;
	*y = VGA(dev)->height;
}

char dvga_get_char(device_t* dev, int x, int y)
{
	return VGA(dev)->memory[(x + y * VGA(dev)->width) * 2];
}

void dvga_set_char(device_t* dev, int x, int y, char c)
{
	VGA(dev)->memory[(x + y * VGA(dev)->width) * 2] = c;
}

void dvga_get_cursor(device_t* dev, int* x, int* y)
{
	*x = VGA(dev)->cursor_x;
	*y = VGA(dev)->cursor_y;
}

void dvga_set_cursor(device_t* dev, int x, int y)
{
	VGA(dev)->cursor_x = x;
	VGA(dev)->cursor_y = y;
	vga_write_cursor_address(VGA(dev));
}

void dvga_scroll(device_t* dev, int lines)
{
	if (lines > 0)
	{
		vga_t* vga = VGA(dev);
		memcpy(vga->memory, vga->memory + (vga->width * 2 * lines),
				vga->width * (vga->height - lines) * 2);
		for (int i = 0; i < vga->width * lines; i++) // We can't use memset because it will mess with the colours
			vga->memory[(vga->width * vga->height - i - lines) * 2] = vga->memory[(vga->width * vga->height - i) * 2];
		VGA(dev)->cursor_y -= lines;
		vga_write_cursor_address(VGA(dev));
	}
}

vga_bus_t vga_bus = {
	.dev = DEV(&vga_dev),
	.get_size = &dvga_get_size,
	.get_char = &dvga_get_char,
	.set_char = &dvga_set_char,
	.get_cursor = &dvga_get_cursor,
	.set_cursor = &dvga_set_cursor,
	.scroll = &dvga_scroll
};

void vga_init()
{
	// Set the CRT controller address to 0x03B4 and 0x03B5
	outb(0x3C2, (inb(0x3CC) & 0b11111110) | 0b00000010);

	module_register(&vga_mod, "vga", NULL, NULL);
	device_register(DEV(&vga_dev), &vga_mod);
	vga_dev.memory = device_mmap((void*) 0xB8000, KB(8));

	device_register_bus(DEV(&vga_dev), VGA, &vga_bus);
}


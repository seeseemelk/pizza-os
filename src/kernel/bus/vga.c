#include "bus/vga.h"

void vga_get_size(vga_bus_t* vga, int* width, int* height)
{
	vga->get_size(vga->dev, width, height);
}

void vga_get_char(vga_bus_t* vga, int x, int y)
{
	vga->get_char(vga->dev, x, y);
}

void vga_set_char(vga_bus_t* vga, int x, int y, char c)
{
	vga->set_char(vga->dev, x, y, c);
}

void vga_get_cursor(vga_bus_t* vga, int* x, int* y)
{
	vga->get_cursor(vga->dev, x, y);
}

void vga_set_cursor(vga_bus_t* vga, int x, int y)
{
	vga->set_cursor(vga->dev, x, y);
}

void vga_scroll(vga_bus_t* vga, int lines)
{
	vga->scroll(vga->dev, lines);
}

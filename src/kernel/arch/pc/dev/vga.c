/*
 * vga.c
 *
 *  Created on: Jan 2, 2018
 *      Author: seeseemelk
 */
#include "vga.h"
#include "../../../dev/devices.h"

module* vga_module;

int vga_request(module_request* request)
{
	return -1;
}

void vga_init()
{
	vga_module = register_module("vga", TERMINAL, vga_request);
}

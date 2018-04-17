#include "ps2.h"
#include "devices.h"
#include "io.h"
#include "cdefs.h"
#include "interrupt.h"
#include "kernel.h"
#include "thread/lock.h"
#include <stdlib.h>
#include <stdio.h>

#define PS2_DAT 0x60
#define PS2_STA 0x64
#define PS2_CMD 0x64

ps2ctrl_t* ps2_get_controller(const device_t* dev)
{
	return (ps2ctrl_t*) dev->data;
}

/**
 * Locks the device.
 * Note that operations can still be performed as normal.
 * The only use it has is to prevent race conditions when
 * using ps2_wait().
 */
void ps2_lock(ps2ctrl_t* dev)
{
	lock_lock(&dev->lock);
}

unsigned char ps2_read_status(ps2ctrl_t* dev)
{
	ps2_lock(dev);
	return inb(PS2_STA);
}

unsigned char ps2_read_resp(ps2ctrl_t* dev)
{
	ps2_lock(dev);
	while ((inb(PS2_STA) & 1) == 0) ;
	return inb(PS2_DAT);
}

unsigned char ps2_read_data(ps2ctrl_t* dev)
{
	ps2_lock(dev);
	return inb(PS2_DAT);
}

void ps2_write_data(ps2ctrl_t* dev, unsigned char data)
{
	ps2_lock(dev);
	outb(PS2_DAT, data);
}

void ps2_write_command(ps2ctrl_t* dev, unsigned char command)
{
	ps2_lock(dev);
	outb(PS2_CMD, command);
}

unsigned char ps2_read_ram(ps2ctrl_t* dev, unsigned char i)
{
	ps2_lock(dev);
	outb(PS2_CMD, 0x20 + i);
	return ps2_read_resp(dev);
}

void ps2_write_ram(ps2ctrl_t* dev, unsigned char i, unsigned char val)
{
	ps2_lock(dev);
	outb(PS2_CMD, 0x60 + i);
	while ((inb(PS2_STA) & 2) == 1);
	outb(PS2_DAT, val);
}

void ps2_wait(ps2ctrl_t* dev)
{
	lock_wait(&dev->lock);
	lock_lock(&dev->lock);
}

int ps2_dev_req(dev_req_t* req)
{
	if (req->type == INTERRUPT)
	{
		interrupt_accept(req->arg2);
		ps2ctrl_t* dev = ps2_get_controller(req->device);
		lock_signal(&dev->lock);
	}
	else
		kernel_panic("PS2: Unknown request type");
	return 0;
}

void ps2_st_init(ps2ctrl_t* dev)
{
	// Disables devices
	ps2_write_command(dev, 0xAD);
	ps2_write_command(dev, 0xA7);

	// Flush output buffer (Wait until bit 0 = 1)
	while ((ps2_read_status(dev) & 1) == 1)
		ps2_read_data(dev);

	// Set Controller Configuration Byte
	unsigned char config = ps2_read_ram(dev, 0);
	dev->num_ports = ((config & (1<<5)) == 1) + 1;
	config &= 0b10111100;
	ps2_write_ram(dev, 0, config);
}

void ps2_init_hardware(ps2ctrl_t* dev)
{
	// Initializes the PS/2 Device
	ps2_st_init(dev);

	// Test PS/2 Controller
	ps2_write_command(dev, 0xAA);
	unsigned char data = ps2_read_resp(dev);
	if (data == 0xFC)
		kernel_panic("PS/2 Controller Bad");
	else if (data != 0x55)
		kernel_panic("PS/2 unknown status flag 0x%X", data);
	kprintf("PS/2 Controller OK\n");

	// Sometimes testing the PS/2 controller causes it to be reset,
	// so we need to initialise it again just to be sage.
	ps2_st_init(dev);

	// Test 1st PS/2 Port
	ps2_write_command(dev, 0xAB);
	data = ps2_read_resp(dev);
	if (data == 1)
		kernel_panic("PS/2 Clock Stuck Low");
	else if (data == 2)
		kernel_panic("PS/2 Clock Stuck High");
	else if (data == 3)
		kernel_panic("PS/2 Data Stuck Low");
	else if (data == 4)
		kernel_panic("PS/2 Data Stuck High");
	kprintf("PS/2 Port 1 OK\n");

	// Enable Port 1
	ps2_write_command(dev, 0xAE);

	// Set Controller Configuration Byte (Enable interrupts)
	unsigned int config = ps2_read_ram(dev, 0);
	config |= 1;
	ps2_write_ram(dev, 0, config);

	// Reset Device
	ps2_write_data(dev, 0xFF);
	unsigned char resp;
	while ((resp = ps2_read_data(dev)) != 0xFA)
	{
		if (resp == 0xFC)
			kernel_panic("PS/2 Device 1 Reset Failure");
		else if (resp == 0xFE)
			ps2_write_data(dev, 0xFF);
	}
	kprintf("PS/2 Device 1 Ready\n");
}

void ps2_init(void)
{
	/*
	 * If ACPI was supported we would have to check if such a controller
	 * is present. ACPI was, however, introduced in 1999, after the release
	 * of the i386.
	 * Therefore ACPI support in PizzaOS is purely optional, if ever implemented.
	 */
	module_t* mod = module_register("ps2", CONTROLLER, NULL, &ps2_dev_req);
	device_t* dev = device_register(mod);
	ps2ctrl_t* ctrl = malloc(sizeof(ps2ctrl_t));
	lock_create(&ctrl->lock);
	dev->data = (void*) ctrl;
	interrupt_register(dev, 0x21);

	// Init the hardware.
	ps2_init_hardware(ctrl);

	// Wait for it to be initialised.
	ps2_wait(ctrl);
}




























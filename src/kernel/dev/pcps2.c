#include "dev/pcps2.h"
#include "bus/ps2.h"
#include "devices.h"
#include "io.h"
#include "cdefs.h"
#include "interrupt.h"
#include "kernel.h"
#include <stdlib.h>
#include <stdio.h>
#include "thread/signal.h"
#include "thread/mutex.h"

#define PS2_DAT 0x60
#define PS2_STA 0x64
#define PS2_CMD 0x64

#define CTRL(bus) ((ps2ctrl_t*)bus)
#define BUS(dev) (&dev->bus)

typedef struct ps2ctrl_t
{
	ps2_bus_t bus;
	int num_ports;
	mutex_t lock;
	signal_t int_signal;
} ps2ctrl_t;

ps2ctrl_t* pcps2_get_controller(const device_t* dev)
{
	return (ps2ctrl_t*) dev->data;
}

void pcps2_lock(ps2_bus_t* dev)
{
	mutex_lock(&CTRL(dev)->lock);
}

void pcps2_unlock(ps2_bus_t* dev)
{
	mutex_unlock(&CTRL(dev)->lock);
}

void pcps2_signal_unset(ps2_bus_t* dev)
{
	signal_unset(&CTRL(dev)->int_signal);
}

void pcps2_signal_wait(ps2_bus_t* dev)
{
	signal_wait(&CTRL(dev)->int_signal);
}

unsigned char pcps2_read_status(ps2_bus_t* dev)
{
	UNUSED(dev);
	return inb(PS2_STA);
}

unsigned char pcps2_read_resp(ps2_bus_t* dev)
{
	UNUSED(dev);
	while ((inb(PS2_STA) & 1) == 0) ;
	return inb(PS2_DAT);
}

unsigned char pcps2_read_data(ps2_bus_t* dev)
{
	UNUSED(dev);
	return inb(PS2_DAT);
}

void pcps2_write_data(ps2_bus_t* dev, unsigned char data)
{
	UNUSED(dev);
	outb(PS2_DAT, data);
}

void pcps2_write_command(ps2_bus_t* dev, unsigned char command)
{
	UNUSED(dev);
	outb(PS2_CMD, command);
}

unsigned char pcps2_read_ram(ps2_bus_t* dev, unsigned char i)
{
	outb(PS2_CMD, 0x20 + i);
	return pcps2_read_resp(dev);
}

void pcps2_write_ram(ps2_bus_t* dev, unsigned char i, unsigned char val)
{
	UNUSED(dev);
	outb(PS2_CMD, 0x60 + i);
	while ((inb(PS2_STA) & 2) == 1);
	outb(PS2_DAT, val);
}

int pcps2_dev_req(dev_req_t* req)
{
	if (req->type == INTERRUPT)
	{
		interrupt_accept(req->arg2);
		ps2ctrl_t* dev = pcps2_get_controller(req->device);
		signal_signal(&dev->int_signal);
		//lock_signal(&dev->lock);
	}
	else
		kernel_panic("PS2: Unknown request type");
	return 0;
}

void pcps2_st_init(ps2ctrl_t* dev)
{
	// Disables devices
	pcps2_write_command(BUS(dev), 0xAD);
	pcps2_write_command(BUS(dev), 0xA7);

	// Flush output buffer (Wait until bit 0 = 1)
	while ((pcps2_read_status(BUS(dev)) & 1) == 1)
		pcps2_read_data(BUS(dev));

	// Set Controller Configuration Byte
	unsigned char config = pcps2_read_ram(BUS(dev), 0);
	dev->num_ports = ((config & (1<<5)) == 1) + 1;
	config &= 0b10111100;
	pcps2_write_ram(BUS(dev), 0, config);
}

void pcps2_init_hardware(ps2ctrl_t* dev)
{
	// Initializes the PS/2 Device
	pcps2_st_init(dev);

	// Test PS/2 Controller
	pcps2_write_command(BUS(dev), 0xAA);
	unsigned char data = pcps2_read_resp(BUS(dev));
	if (data == 0xFC)
		kernel_panic("PS/2 Controller Bad");
	else if (data != 0x55)
		kernel_panic("PS/2 unknown status flag 0x%X", data);
	kprintf("PS/2 Controller OK\n");

	// Sometimes testing the PS/2 controller causes it to be reset,
	// so we need to initialise it again just to be sage.
	pcps2_st_init(dev);

	// Test 1st PS/2 Port
	pcps2_write_command(BUS(dev), 0xAB);
	data = pcps2_read_resp(BUS(dev));
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
	pcps2_write_command(BUS(dev), 0xAE);

	// Set Controller Configuration Byte (Enable interrupts)
	unsigned int config = pcps2_read_ram(BUS(dev), 0);
	config |= 1;
	pcps2_write_ram(BUS(dev), 0, config);

	// Reset Device
	pcps2_write_data(BUS(dev), 0xFF);
	unsigned char resp;
	while ((resp = pcps2_read_data(BUS(dev))) != 0xFA)
	{
		if (resp == 0xFC)
			kernel_panic("PS/2 Device 1 Reset Failure");
		else if (resp == 0xFE)
			pcps2_write_data(BUS(dev), 0xFF);
	}
	kprintf("PS/2 Device 1 Ready\n");
}

void pcps2_init(void)
{
	/*
	 * If ACPI was supported we would have to check if such a controller
	 * is present. ACPI was, however, introduced in 1999, after the release
	 * of the i386.
	 * Therefore ACPI support in PizzaOS is purely optional, if ever implemented.
	 */
	/*module_t* mod = module_register("ps2", PS2, NULL, &pcps2_dev_req);
	device_t* dev = device_register(mod);
	ps2ctrl_t* ctrl = malloc(sizeof(ps2ctrl_t));
	signal_new(&ctrl->int_signal);
	dev->data = (void*) ctrl;
	interrupt_register(dev, 0x21);

	ctrl->bus.read_status = pcps2_read_status;
	ctrl->bus.read_data = pcps2_read_data;
	ctrl->bus.write_data = pcps2_write_data;
	ctrl->bus.write_command = pcps2_write_command;
	ctrl->bus.wait = pcps2_signal_wait;

	// Initialises the hardware.
	pcps2_init_hardware(ctrl);

	// Wait for it to be initialised.
	pcps2_signal_wait(BUS(ctrl));

	ps2_register(dev, &ctrl->bus);*/
}




























#include "dev/pcps2.h"
#include "bus/ps2.h"
#include "devices.h"
#include "io.h"
#include "cdefs.h"
#include "interrupt.h"
#include "kernel.h"
#include "thread/lock.h"
#include <stdlib.h>

#define PS2_DAT 0x60
#define PS2_STA 0x64
#define PS2_CMD 0x64

#define CTRL(dev) ((ps2ctrl_t*)dev)
#define DEV(ctrl) ((device_t*)ctrl)

typedef struct
{
	device_t* dev;
	int num_ports;
	lock_t lock;
	ps2_bus_t bus;
} ps2ctrl_t;

/**
 * Locks the device.
 * Note that operations can still be performed as normal.
 * The only use it has is to prevent race conditions when
 * using ps2_wait().
 */
void pcps2_lock(device_t* dev)
{
	UNUSED(dev);
	//lock_lock(&CTRL(dev)->lock);
}

unsigned char pcps2_read_status(device_t* dev)
{
	pcps2_lock(dev);
	return inb(PS2_STA);
}

unsigned char pcps2_read_resp(device_t* dev)
{
	pcps2_lock(dev);
	while ((inb(PS2_STA) & 1) == 0) ;
	return inb(PS2_DAT);
}

unsigned char pcps2_read_data(device_t* dev)
{
	pcps2_lock(dev);
	return inb(PS2_DAT);
}

void pcps2_write_data(device_t* dev, unsigned char data)
{
	pcps2_lock(dev);
	outb(PS2_DAT, data);
}

void pcps2_write_command(device_t* dev, unsigned char command)
{
	pcps2_lock(dev);
	outb(PS2_CMD, command);
}

unsigned char pcps2_read_ram(device_t* dev, unsigned char i)
{
	pcps2_lock(dev);
	outb(PS2_CMD, 0x20 + i);
	return pcps2_read_resp(dev);
}

void pcps2_write_ram(device_t* dev, unsigned char i, unsigned char val)
{
	pcps2_lock(dev);
	outb(PS2_CMD, 0x60 + i);
	while ((inb(PS2_STA) & 2) == 1);
	outb(PS2_DAT, val);
}

void pcps2_wait(device_t* dev)
{
	UNUSED(dev);
	//lock_wait(&CTRL(dev)->lock);
	//lock_lock(&CTRL(dev)->lock);
}

int pcps2_dev_req(dev_req_t* req)
{
	if (req->type == INTERRUPT)
	{
		//interrupt_accept(req->arg2);
		//lock_signal(&dev->lock);
		return INT_ACCEPT;
	}
	else
		kernel_panic("PS2: Unknown request type");
	return 0;
}

void pcps2_st_init(ps2ctrl_t* dev)
{
	// Disables devices
	pcps2_write_command(DEV(dev), 0xAD);
	pcps2_write_command(DEV(dev), 0xA7);

	// Flush output buffer (Wait until bit 0 = 1)
	while ((pcps2_read_status(DEV(dev)) & 1) == 1)
		pcps2_read_data(DEV(dev));

	// Set Controller Configuration Byte
	unsigned char config = pcps2_read_ram(DEV(dev), 0);
	dev->num_ports = ((config & (1<<5)) == 1) + 1;
	config &= 0b10111100;
	pcps2_write_ram(DEV(dev), 0, config);
}

void pcps2_init_hardware(ps2ctrl_t* dev)
{
	// Initializes the PS/2 Device
	pcps2_st_init(dev);

	// Test PS/2 Controller
	pcps2_write_command(DEV(dev), 0xAA);
	unsigned char data = pcps2_read_resp(DEV(dev));
	if (data == 0xFC)
		kernel_panic("PS/2 Controller Bad");
	else if (data != 0x55)
		kernel_panic("PS/2 unknown status flag 0x%X", data);
	kernel_log("PS/2 Controller OK");

	// Sometimes testing the PS/2 controller causes it to be reset,
	// so we need to initialise it again just to be sage.
	pcps2_st_init(dev);

	// Test 1st PS/2 Port
	pcps2_write_command(DEV(dev), 0xAB);
	data = pcps2_read_resp(DEV(dev));
	if (data == 1)
		kernel_panic("PS/2 Clock Stuck Low");
	else if (data == 2)
		kernel_panic("PS/2 Clock Stuck High");
	else if (data == 3)
		kernel_panic("PS/2 Data Stuck Low");
	else if (data == 4)
		kernel_panic("PS/2 Data Stuck High");
	kernel_log("PS/2 Port 1 OK");

	// Enable Port 1
	pcps2_write_command(DEV(dev), 0xAE);

	// Set Controller Configuration Byte (Enable interrupts)
	unsigned int config = pcps2_read_ram(DEV(dev), 0);
	config |= 1;
	pcps2_write_ram(DEV(dev), 0, config);

	// Reset Device
	pcps2_write_data(DEV(dev), 0xFF);
	unsigned char resp;
	while ((resp = pcps2_read_data(DEV(dev))) != 0xFA)
	{
		if (resp == 0xFC)
			kernel_panic("PS/2 Device 1 Reset Failure");
		else if (resp == 0xFE)
			pcps2_write_data(DEV(dev), 0xFF);
	}
	kernel_log("PS/2 Device 1 Ready");
}

void pcps2_init(void)
{
	/*
	 * If ACPI was supported we would have to check if such a controller
	 * is present. ACPI was, however, introduced in 1999, after the release
	 * of the i386.
	 * Therefore ACPI support in PizzaOS is purely optional, if ever implemented.
	 */
	module_t* mod = malloc(sizeof(module_t));
	ps2ctrl_t* ctrl = malloc(sizeof(ps2ctrl_t));
	module_register(mod, "ps2", NULL, &pcps2_dev_req);
	device_register(DEV(ctrl), mod);

	interrupt_register(DEV(ctrl), 0x21);

	ctrl->bus.read_status = pcps2_read_status;
	ctrl->bus.read_data = pcps2_read_data;
	ctrl->bus.write_data = pcps2_write_data;
	ctrl->bus.write_command = pcps2_write_command;
	ctrl->bus.wait = pcps2_wait;

	// Initialises the hardware.
	pcps2_init_hardware(ctrl);

	// Wait for it to be initialised.
	pcps2_wait(DEV(ctrl));

	//ps2_register(ctrl, &ctrl->bus);
	device_register_bus(DEV(ctrl), PS2, &ctrl->bus);
}




























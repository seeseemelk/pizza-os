#include "dev/pcps2.h"
#include "bus/ps2.h"
#include "devices.h"
#include "io.h"
#include "cdefs.h"
#include "interrupt.h"
#include "kernel.h"
#include "thread/signal.h"
#include "thread/mutex.h"
#include <stdlib.h>
#include <stdbool.h>

#define PS2_DAT 0x60
#define PS2_STA 0x64
#define PS2_CMD 0x64

#define CTRL(dev) ((ps2ctrl_t*)dev)
#define DEV(ctrl) ((device_t*)ctrl)

typedef struct
{
	device_t* dev;
	int num_ports;
	ps2_bus_t bus;
	signal_t signal;
	bool wait_interrupt; /* Setting this to true will make this driver accept interrupts,
							setting it to false will cause it to ignore interrupts. */
} ps2ctrl_t;

unsigned char pcps2_read_status(device_t*);

/**
 * Waits until the PS2 input buffer is empty.
 * This should be done before writing. (NOT reading!)
 */
void pcps2_wait_input_clear(device_t* dev)
{
	while ((pcps2_read_status(dev) & 0x02) > 0);
}

unsigned char pcps2_read_status(device_t* dev)
{
	UNUSED(dev);
	return inb(PS2_STA);
}

unsigned char pcps2_read_resp(device_t* dev)
{
	UNUSED(dev);
	while ((inb(PS2_STA) & 1) == 0) ;
	return inb(PS2_DAT);
}

unsigned char pcps2_read_data(device_t* dev)
{
	UNUSED(dev);
	return inb(PS2_DAT);
}

void pcps2_write_data(device_t* dev, unsigned char data)
{
	UNUSED(dev);
	pcps2_wait_input_clear(dev);
	outb(PS2_DAT, data);
}

void pcps2_write_command(device_t* dev, unsigned char command)
{
	UNUSED(dev);
	pcps2_wait_input_clear(dev);
	outb(PS2_CMD, command);
}

unsigned char pcps2_read_ram(device_t* dev, unsigned char i)
{
	//outb(PS2_CMD, 0x20 + i);
	pcps2_write_command(dev, 0x20 + i);
	return pcps2_read_resp(dev);
}

void pcps2_write_ram(device_t* dev, unsigned char i, unsigned char val)
{
	pcps2_wait_input_clear(dev);
	outb(PS2_CMD, 0x60 + i);
	pcps2_wait_input_clear(dev);
	outb(PS2_DAT, val);
}

int pcps2_dev_req(dev_req_t* req)
{
	if (req->type == INTERRUPT)
	{
		ps2ctrl_t* ctrl = CTRL(req->device);
		if (ctrl->wait_interrupt)
		{
			signal_signal(&ctrl->signal);
			ctrl->wait_interrupt = false;
			return INT_ACCEPT;
		}
		else
			return INT_IGNORE;
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
	dev->num_ports = ((config & (1<<5)) > 0) + 1;
	config &= 0b10111100;

	pcps2_write_ram(DEV(dev), 0, config);
}

void pcps2_init_hardware(ps2ctrl_t* dev)
{
	signal_t* signal = &dev->signal;
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
	// so we need to initialise it again just to be safe.
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
	dev->wait_interrupt = true;
	signal_clear(signal);
	pcps2_write_data(DEV(dev), 0xFF);
	unsigned char resp;
	while ((resp = pcps2_read_data(DEV(dev))) != 0xFA)
	{
		if (resp == 0xFC)
			kernel_panic("PS/2 Device 1 Reset Failure");
		else if (resp == 0xFE)
			pcps2_write_data(DEV(dev), 0xFF);
		dev->wait_interrupt = true;
	}
	signal_wait(signal);
	kernel_log("PS/2 Device 1 Reset and ready");
}

void pcps2_init(void)
{
	kernel_log("Initialising PS2 controller");
	/*
	 * If ACPI was supported we would have to check if such a controller
	 * is present. ACPI was, however, introduced in 1999, after the release
	 * of the i386.
	 * Therefore ACPI support in PizzaOS is purely optional, if ever implemented.
	 */
	module_t* mod = malloc(sizeof(module_t));
	ps2ctrl_t* ctrl = malloc(sizeof(ps2ctrl_t));
	signal_new(&ctrl->signal);
	ctrl->num_ports = 0;
	ctrl->wait_interrupt = false;

	module_register(mod, "ps2", NULL, &pcps2_dev_req);
	device_register(DEV(ctrl), mod);

	interrupt_register(DEV(ctrl), 0x21);

	ctrl->bus.read_status = pcps2_read_status;
	ctrl->bus.read_data = pcps2_read_data;
	ctrl->bus.write_data = pcps2_write_data;
	ctrl->bus.write_command = pcps2_write_command;

	// Initialises the hardware.
	pcps2_init_hardware(ctrl);

	// Wait for it to be initialised.
	//pcps2_wait(DEV(ctrl));

	device_register_bus(DEV(ctrl), PS2, &ctrl->bus);
	kernel_log("Initialised PS2 controller");
}




























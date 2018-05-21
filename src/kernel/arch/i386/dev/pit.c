#include "arch/i386/dev/pit.h"
#include "kernel.h"
#include "devices.h"
#include "interrupt.h"
#include "cdefs.h"
#include "sched.h"
#include "io.h"
#include "config.h"

#define PORT0 0x40
#define COMMAND 0x43

typedef struct
{
	module_t mod;
} pit_mod;

typedef struct
{
	device_t dev;
	unsigned long long delay;
} pit_dev;

typedef struct __attribute__((packed))
{
	char bcd : 1;
	char mode : 3;
	char access : 2;
	char channel : 2;
} command_t;

static pit_mod mod;
static pit_dev dev;

int pit_req(dev_req_t* req)
{
	if (req->type == INTERRUPT)
	{
		kernel_time_add(dev.delay);
		sched_notify((irq_t*) req->arg1, dev.delay);
		return INT_ACCEPT;
	}
	else
		kernel_panic("Unknown request type to PIT");
	return 0;
}

void pit_init()
{
	module_register(&mod.mod, "pit", NULL, pit_req);
	device_register(&dev.dev, &mod.mod);
	interrupt_register(&dev.dev, 0x20);

	interrupt_disable();
	// Setup channel 0
	command_t cmd = {
		.bcd = 0,
		.mode = 3,
		.access = 3,
		.channel = 0
	};
	//char command = 0xC3;
	kernel_log("Setting to mode 0x%X", *(u8*)&cmd);
	// Use 59635 for 50 milliseconds exact
	// This is just shy of 1 millisecond
	unsigned short divisor = PIT_INTERVAL;
	//unsigned short divisor = 50;
	dev.delay = (1000000ULL * divisor) / 1193180;
	outb(COMMAND, *(u8*)&cmd);
	outb(PORT0, (divisor) & 0xFF);
	outb(PORT0, (divisor >> 8) & 0xFF);
	interrupt_enable();
}









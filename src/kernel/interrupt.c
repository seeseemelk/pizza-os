/*
 * interrupts.c
 *
 *  Created on: Jan 24, 2018
 *      Author: seeseemelk
 */

#include "interrupt.h"
#include "config.h"
#include "kernel.h"
#include "devices.h"

#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>

typedef struct int_handler int_handler;

device** handlers[256]; /* Contains an array of pointers to devices (handlers) for each interrupt */
int num_handlers[256]; /* Contains the number of handlers registered for each interrupt */
int int_count = 0; /* A counter that can be used to individually track interrupts */
int last_handled = -1; /* The id of the last handled interrupt */

void interrupt_register(device* dev, int interrupt)
{
	if (num_handlers[interrupt] < MAX_NUM_INT_HANDLERS)
		handlers[interrupt][num_handlers[interrupt]++] = dev;
	else
		kernel_panic("Too many handlers have been registered for interrupt 0x%X", interrupt);
}

void interrupt_init()
{
	for (int i = 0; i < 256; i++)
	{
		handlers[i] = malloc(sizeof(device*) * MAX_NUM_INT_HANDLERS);
		num_handlers[i] = 0;
	}

	arch_interrupt_init();
}

void interrupt_handle(int irq, int code)
{
	if (irq == 0xD) // General protection fault
	{
		kernel_panic("General Protection Fault (error: 0x%X)", code);
	}
	else if (irq == 0xE) // Page fault
	{
		kernel_panic("Page Fault (error: 0x%X)", code);
	}
	else
	{
		int id = int_count++;
		device** devices = handlers[irq];
		int num_devices = num_handlers[irq];
		for (int i = 0; i < num_devices; i++)
		{
			device* dev = devices[i];
			device_invoke2(dev, INTERRUPT, irq, id);
			if (last_handled == id)
				return;
		}
		kprintf("Unhandled interrupt 0x%X\n", irq);
	}
}

void interrupt_accept(int interrupt_id)
{
	last_handled = interrupt_id;
}


















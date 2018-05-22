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

device_t** handlers[256]; /* Contains an array of pointers to devices (handlers) for each interrupt */
int num_handlers[256]; /* Contains the number of handlers registered for each interrupt */

void interrupt_register(device_t* dev, int interrupt)
{
	if (dev->module->fn_dev_req == NULL)
		kernel_log("Module %s registered int 0x%X without fn_dev_req", dev->module->name, interrupt);

	if (num_handlers[interrupt] < MAX_NUM_INT_HANDLERS)
		handlers[interrupt][num_handlers[interrupt]++] = dev;
	else
		kernel_panic("Too many handlers - too much pizza - have been registered for interrupt 0x%X", interrupt);
}

void interrupt_init()
{
	for (int i = 0; i < 256; i++)
	{
		handlers[i] = malloc(sizeof(device_t*) * MAX_NUM_INT_HANDLERS);
		num_handlers[i] = 0;
	}

	arch_interrupt_init();
}

void interrupt_handle(int int_num, int code)
{
	irq_t irq = {
		.num = int_num,
		.error_code = code,
		.finished = false
	};

	if (irq.num == 0x6) // Invalid opcode
	{
		kernel_panic("Invalid opcode - This ain't pizza?");
	}
	else if (irq.num == 0xD) // General protection fault
	{
		kernel_panic("General Protection Fault (error: 0x%X) - Forgot how to eat pizza?", code);
	}
	else if (irq.num == 0xE) // Page fault
	{
		kernel_panic("Page Fault (error: 0x%X) - No pizza on this plate", code);
	}
	else
	{
		device_t** devices = handlers[irq.num];
		int num_devices = num_handlers[irq.num];
		for (int i = 0; i < num_devices; i++)
		{
			device_t* dev = devices[i];
			irq_status_t status = device_invoke1(dev, INTERRUPT, (int) &irq);
			if (status == INT_ACCEPT)
			{
				interrupt_finish(&irq);
				return;
			}
		}
		kernel_log("Unhandled interrupt 0x%X", irq.num);
		interrupt_finish(&irq);
	}
}

void interrupt_finish(irq_t* irq)
{
	if (!irq->finished)
	{
		irq->finished = true;
		arch_interrupt_finish(irq);
	}
}


















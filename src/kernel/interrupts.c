/*
 * interrupts.c
 *
 *  Created on: Jan 24, 2018
 *      Author: seeseemelk
 */

#include "interrupts.h"
#include "config.h"

#include <stdlib.h>
#include <stddef.h>

typedef struct int_handler int_handler;

device** handlers[256];
int num_handlers[256];

void interrupt_register(device* dev, int interrupt)
{
	handlers[interrupt][num_handlers[interrupt]] = dev;
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

/*
 * interrupts.h
 *
 *  Created on: Jan 23, 2018
 *      Author: seeseemelk
 */

#ifndef INTERRUPTS_H_
#define INTERRUPTS_H_

#include "dev/devices.h"

typedef enum int_result_t int_result_t;
typedef int_result_t(interrupt_f)(int);

enum int_result_t
{
	INT_END, INT_CONTINUE
};

void interrupt_register(device* dev, int interrupt);
void interrupt_init();

/**
 * Lets the chosen architecture initialize interrupt.
 * This is called automatically by interrupt_init().
 */
void arch_interrupt_init();


#endif /* INTERRUPTS_H_ */

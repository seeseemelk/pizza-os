/*
 * pic.h
 *
 *  Created on: Jan 25, 2018
 *      Author: seeseemelk
 */

#ifndef ARCH_I386_I386_PIC_H_
#define ARCH_I386_I386_PIC_H_

#include "cdefs.h"
#include <stdbool.h>

void pic_send_eoi(u8 irq);

/**
 * Checks if an interrupt is currently being served.
 * @param irq The IRQ to check for.
 * @return true if that IRQ is currently being served to the CPU, false if it isn't.
 */
bool pic_is_served(u8 irq);
void pic_init();

#endif /* ARCH_I386_I386_PIC_H_ */

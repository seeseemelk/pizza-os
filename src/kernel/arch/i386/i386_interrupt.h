/*
 * interrupt.h
 *
 *  Created on: Jan 24, 2018
 *      Author: seeseemelk
 */

#ifndef ARCH_I386_I386_INTERRUPT_H_
#define ARCH_I386_I386_INTERRUPT_H_

#include "i386_interrupt.h"

void interrupt_debug_jmp(int exceptionNumber, void* target);

#endif /* ARCH_I386_I386_INTERRUPT_H_ */

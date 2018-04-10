/*
 * cpu.h
 *
 *  Created on: Apr 10, 2018
 *      Author: seeseemelk
 */

#ifndef CPU_H_
#define CPU_H_

#if TARGET==i386
#include "arch/i386/i386_cpu.h"
#endif

/**
 * Pauses the CPU until the next interrupt.
 */
void cpu_pause();

/**
 * Initialises the CPU.
 */
void cpu_init();

#endif /* CPU_H_ */

/*
 * i386_threads.h
 *
 *  Created on: Apr 10, 2018
 *      Author: seeseemelk
 */

#ifndef ARCH_I386_I386_THREADS_H_
#define ARCH_I386_I386_THREADS_H_
//#include "thread.h"
#include "arch/i386/i386_asm.h"

typedef struct thread_data thread_data;
typedef struct thread_t thread_t;

/**
 * @internal
 */
struct thread_data
{
	u16 ds;
	u16 ss;
	u16 cs;
	u32 eip;
	u32 eflags;
	u32 esp;
	u32 ebp;
	u32 esi;
	u32 edi;
	void* stack;
} __attribute__((packed));


#endif /* ARCH_I386_I386_THREADS_H_ */

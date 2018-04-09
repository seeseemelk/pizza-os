#include "thread.h"
#include "kernel.h"
#include "cdefs.h"
#include "kernel.h"
#include "config.h"
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>

thread_t* threads;
size_t next_thread_id = 1;
thread_t* current_thread;

/**
 * Saves the current thread's registers to a given thread_data struct.
 */
void thread_start()
{
	current_thread->entry_point();
	kernel_panic("Reached end of thread");
}

void thread_enter(thread_data* data)
{
	asm volatile (
			// Restore EBP and ESP so we can use the stack.
			"movl 30(%0), %%eax;"
			"movl %%eax, %%esp;"
			"movl 34(%0), %%eax;"
			"movl %%eax, %%ebp;"

			// Restore DS and SS
			"movl 0(%0), %%eax;"
			"movl %%eax, %%ds;"
			"movl 2(%0), %%eax;"
			"movl %%eax, %%ss;"

			// Load the values so we can load with pushal
			"movl %0, %%eax;"
			"add $4, %%eax;"
			//"pushw (%%eax);" // We don't use CS (yet)
			"add $2, %%eax;"
			"pushl (%%eax);"
			"add $4, %%eax;"
			"pushl (%%eax);"
			"add $4, %%eax;"
			"pushl (%%eax);"
			"add $4, %%eax;"
			"pushl (%%eax);"
			"add $4, %%eax;"
			"pushl (%%eax);"
			"add $4, %%eax;"
			"pushl (%%eax);"
			"add $4, %%eax;"
			"pushl (%%eax);"
			"add $4, %%eax;"
			"pushl (%%eax);"
			"add $4, %%eax;"
			"pushl (%%eax);"
			"add $4, %%eax;"
			"pushl (%%eax);"
			"popal;" // Restore the values now from the stack.
			"popfl;"

			// Perform far jump
			"ret;"
			:
			: "r" (data)
			: "eax"

	);
}

void thread_save(thread_data* data)
{
	asm volatile (
			"movl %%esp, %0;"
			"movl %%ebp, %1;"
			: "=m" (data->esp), "=m" (data->ebp)
	);
	asm volatile (
			"movl %%esi, %0;"
			"movl %%edi, %1;"
			"movw %%ds, %%ax; movw %%ax, %2;"
			"movw %%ss, %%ax; movw %%ax, %3;"
			"movw %%cs, %%ax; movw %%ax, %4;"
			"movl $1f, %5;"
			"1:"
			: "=m" (data->esi), "=m" (data->edi), "=m" (data->cs), "=m" (data->ss), "=m" (data->ds),
			  "=m" (data->eip)
	);
}

void thread_switch(thread_t* thread)
{
	thread_save(&current_thread->data);
	current_thread = thread;
	thread_enter(&thread->data);
}

/**
 * Real entry point for any thread.
 */
thread_t* thread_create(void(*entry_point)(void))
{
	thread_t* thread = NULL;
	// Find a free thread
	for (int i = 0; i < 64; i++)
	{
		if (threads[i].id == 0)
		{
			thread = threads + i;
			break;
		}
	}
	if (thread == NULL)
		kernel_panic("Too many threads");

	thread->entry_point = entry_point;
	thread->id = next_thread_id++;
	thread_data* data = &thread->data;
	thread_save(data);
	data->stack = malloc(4096);
	data->ebp = (u32)((char*)data->stack + 4092);
	data->esp = (u32)((char*)data->stack + 4092);
	data->eip = (u32) thread_start;

	return thread;
}

void thread_free(thread_t* thread)
{
	kprintf("Freeing %d, current is %d\n", thread->id, current_thread->id);
	if (current_thread->id == thread->id)
		kernel_panic("Trying to free currently running thread");
	free(thread->data.stack);
	thread->id = 0;
}

void thread_leave()
{
	thread_switch(threads);
}

void sched_run();

void thread1()
{
	while(1)
	{
		kprintf("From thread 1\n");
		thread_leave();
	}
}

void thread2()
{
	while(1)
	{
		kprintf("From thread 2\n");
		thread_leave();
	}
}

void thread_init()
{
	threads = calloc(sizeof(thread_t), MAX_THREADS);

	/* Stores this call chain as a thread. */
	current_thread = threads;
	current_thread->id = next_thread_id++;
	thread_save(&current_thread->data);

	thread_create(&thread1);
	thread_create(&thread2);

	sched_run();
}

void sched_run()
{
	while (1)
	{
		int run = 0;
		for (int i = 1; i < MAX_THREADS; i++)
		{
			thread_t* thread = threads + i;
			if (thread->id != 0)
			{
				thread_switch(thread);
				run++;
			}
		}
		if (run == 0)
		{
			kernel_panic("Last thread died");
		}
	}
}
















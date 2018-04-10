#include "kernel.h"
#include "cdefs.h"
#include "kernel.h"
#include "config.h"
#include "interrupt.h"
#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <threads.h>

// For some reason it won't read the struct from thread.h
#if TARGET==i386
#include "arch/i386/i386_threads.h"
#endif

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

void thread_switch(thread_t* thread)
{
	interrupt_disable();
	thread_save(current_thread->data);
	current_thread = thread;
	thread_enter(thread->data);
	interrupt_enable();
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
	thread_data* data = thread->data;
	thread_save(data);
	data->stack = malloc(4096);
	data->ebp = (u32)((char*)data->stack + 4092);
	data->esp = (u32)((char*)data->stack + 4092);
	data->eip = (u32) thread_start;
	*((char*)data->esp) = (u32) thread_start;

	return thread;
}

void thread_free(thread_t* thread)
{
	kprintf("Freeing %d, current is %d\n", thread->id, current_thread->id);
	if (current_thread->id == thread->id)
		kernel_panic("Trying to free currently running thread");
	free(thread->data->stack);
	thread->id = 0;
}

void thread_leave()
{
	thread_switch(threads);
}

void thread_init()
{
	threads = calloc(sizeof(thread_t), MAX_THREADS);
	thread_data* datas = calloc(sizeof(thread_data), MAX_THREADS);
	for (int i = 0; i < MAX_THREADS; i++)
	{
		threads[i].data = datas + i;
	}

	/* Stores this call chain as a thread. */
	current_thread = threads;
	current_thread->id = next_thread_id++;
	thread_save(current_thread->data);
}

void thread_iterator_create(thread_it* it)
{
	it->thread = threads;
	it->index = 0;
}

bool thread_iterator_has_next(thread_it* it)
{
	return it->thread != NULL;
}

thread_t* thread_iterator_next(thread_it* it)
{
	thread_t* thread_to_return = it->thread;
	it->thread = NULL;
	for (int i = it->index + 1; i < MAX_THREADS; i++)
	{
		if (threads[i].id != 0)
		{
			it->thread = threads + i;
			it->index = i;
			break;
		}
	}
	return thread_to_return;
}
















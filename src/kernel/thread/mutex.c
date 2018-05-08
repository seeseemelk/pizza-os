#include "threads.h"
#include "interrupt.h"
#include "thread/mutex.h"

void mutex_new(mutex_t* mutex)
{
	mutex->waiting_threads = list_new();
	mutex->locked = false;
}

void mutex_free(mutex_t* mutex)
{
	list_free(mutex->waiting_threads);
}

void mutex_lock(mutex_t* mutex)
{
	interrupt_disable();
	if (mutex->locked)
	{
		list_add(mutex->waiting_threads, current_thread);
		size_t index = list_size(mutex->waiting_threads);
		while(mutex->locked)
		{
			interrupt_enable();
			thread_leave();
			interrupt_disable();
		}
		list_remove(mutex->waiting_threads, index);
	}

	mutex->locked = true;
	interrupt_enable();
}

void mutex_unlock(mutex_t* mutex)
{
	interrupt_disable();
	size_t size = list_size(mutex->waiting_threads);
	for (size_t i = 0; i < size; i++)
	{
		thread_t* thread = list_get(mutex->waiting_threads, i);
		thread_set_paused(thread, false);
	}
	interrupt_enable();
}

#include "threads.h"
#include "interrupt.h"
#include "thread/mutex.h"

/*
 * Mutexes could be improved by switched the
 * array-backed list implementation by a
 * linked list, as only the first element
 * is of any importance.
 */

void mutex_new(mutex_t* mutex)
{
	mutex->waiting_threads = list_new();
	mutex->signal = 0;
	mutex->waiting = 0;
}

void mutex_free(mutex_t* mutex)
{
	list_free(mutex->waiting_threads);
}

void mutex_lock(mutex_t* mutex)
{
	interrupt_disable();
	int signal = mutex->waiting++;
	list_add(mutex->waiting_threads, current_thread);
	while(mutex->signal != signal)
	{
		thread_set_paused(current_thread, true);
		interrupt_enable();
		thread_leave();
		interrupt_disable();
	}
	thread_set_paused(current_thread, false);
	list_remove(mutex->waiting_threads, 0);

	interrupt_enable();
}

void mutex_unlock(mutex_t* mutex)
{
	interrupt_disable();
	size_t size = list_size(mutex->waiting_threads);
	if (size > 0)
	{
		thread_t* thread = list_get(mutex->waiting_threads, 0);
		thread_set_paused(thread, false);
	}
	mutex->signal++;
	interrupt_enable();
}

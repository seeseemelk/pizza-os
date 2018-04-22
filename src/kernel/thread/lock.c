/*
 * lock.c
 *
 *  Created on: Apr 10, 2018
 *      Author: seeseemelk
 */
#include "lock.h"
#include "threads.h"
#include "kernel.h"
#include "interrupt.h"

void lock_create(lock_t* lock)
{
	lock->owner = NULL;
	lock->signal = false;
}

void lock_lock(lock_t* lock)
{
	lock->owner = current_thread;
	lock->signal = false;
}

void lock_unlock(lock_t* lock)
{
	lock->signal = true;
}

void lock_wait(lock_t* lock)
{
	lock->owner = current_thread;
	while (!lock->signal)
	{
		thread_set_paused(lock->owner, true);
		thread_leave();
	}
}

void lock_signal(lock_t* lock)
{
	if (lock->owner != NULL)
	{
		lock->signal = true;
		thread_set_paused(lock->owner, false);
	}
	else
		kernel_panic("Signaling un-owned lock - Can't wait until finished eating pizza if not eating pizza");
}

bool lock_is_locked(lock_t* lock)
{
	return lock->owner != NULL;
}

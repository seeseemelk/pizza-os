/*
 * mutex.h
 *
 *  Created on: Apr 25, 2018
 *      Author: seeseemelk
 */

#ifndef THREAD_MUTEX_H_
#define THREAD_MUTEX_H_

#include "collections/list.h"

typedef struct mutex_t mutex_t;
struct mutex_t
{
	list_t* waiting_threads;
	int signal;
	int waiting;
};

/**
 * Creates a new mutex.
 * This method has to be called before
 * a mutex can be used.
 * @param mutex The mutex to initialise.
 */
void mutex_new(mutex_t* mutex);

/**
 * Frees a mutex.
 * This should be called when a mutex
 * is not needed any longer.
 * @param mutex To mutex to free.
 */
void mutex_free(mutex_t* mutex);

/**
 * Locks a mutex.
 * Only one thread can lock a mutex at any time.
 * Other threads will block until the mutex is freed again.
 * Threads will resume in the same order as they acquired
 * their locks.
 * @param mutex The mutex to lock.
 */
void mutex_lock(mutex_t* mutex);

/**
 * Unlocks a mutex.
 * This will allow other threads to start using the mutex.
 * @param mutex The mutex to unlock
 */
void mutex_unlock(mutex_t* mutex);

#endif /* THREAD_MUTEX_H_ */

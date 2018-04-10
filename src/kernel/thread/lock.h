/**
 * @file
 * Contains a locking primitive.
 * A thread can lock on it which will cause that thread to be paused
 * until another thread signals the lock.
 * At that point the lock is unlocked and the thread will resume.
 * @note Only one lock can be waiting on the lock at any one time.
 */

#ifndef THREAD_LOCK_H_
#define THREAD_LOCK_H_
#include "threads.h"
#include <stdbool.h>

/**
 * Lock type definition.
 */
typedef struct lock_t lock_t;

struct lock_t
{
	thread_t* owner;
	bool signal;
};

/**
 * Creates a lock.
 * @param lock The lock to create.
 */
void lock_create(lock_t* lock);

/**
 * Waits until the lock is signaled.
 * @param lock The lock to wait for.
 */
void lock_wait(lock_t* lock);

/**
 * Signals the lock.
 */
void lock_signal(lock_t* lock);

/**
 * Checks if a thread is currently locked by the lock.
 * @param lock The lock to check for.
 * @return `true` if a thread is locked by it, `false` if there is no locked thread.
 */
bool lock_is_locked(lock_t* lock);

#endif /* THREAD_LOCK_H_ */

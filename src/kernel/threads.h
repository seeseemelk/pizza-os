/**
 * @file
 * Contains threading-related functions.
 */

#ifndef THREADS_H_
#define THREADS_H_
#include "cdefs.h"
#include <stdbool.h>

/**
 * Contains information necessary to perform context switches.
 * It is to be defined by the architecture dependent code.
 */
typedef struct thread_data thread_data;

/**
 * Contains information about a thread.
 */
typedef struct thread_t thread_t;

/**
 * Thread iterator.
 * Is used to iterator over all existing threads.
 */
typedef struct thread_it thread_it;

/**
 * Represents a single thread.
 */
struct thread_t
{
	int id; /*< The unique ID of the thread to run. */
	void(*entry_point)(void); /*< The entry point of the thread. */
	thread_data* data; /*< Contains important data required for performing context switches */
};

/**
 * Thread iterator.
 * All members are used internally.
 */
struct thread_it
{
	thread_t* thread;
	int index;
};

#if TARGET==i386
#include "arch/i386/i386_threads.h"
#endif

/**
 * Always points to the currently running thread.
 */
extern thread_t* current_thread;

/**
 * Creates a new thread.
 * When the thread is ran, entry_point will be executed.
 * @param entry_point The function to execute on the thread.
 * @return The thread object.
 */
thread_t* thread_create(void(*entry_point)(void));

/**
 * Frees a thread.
 * @note This function cannot be called from the thread to be freed.
 * @param thread The thread to free.
 */
void thread_free(thread_t* thread);

/**
 * Switches to a different thread.
 * @param thread The thread to switch to.
 */
void thread_switch(thread_t* thread);

/**
 * Gets a thread by it's ID.
 * @param id The ID of the thread to get.
 * @return The thread or `NULL` if thread with the given ID exists.
 */
thread_t* thread_get(int id);

/**
 * Creates a new iterator.
 * @param it The struct to store the iterator in.
 */
void thread_iterator_create(thread_it* it);

/**
 * Checks if a next thread exists.
 * @param it The iterator to use.
 * @return `true` if there is a next thread, `false` if the end of the thread pool has been reached.
 */
bool thread_iterator_has_next(thread_it* it);

/**
 * Gets the next existing thread.
 * @param it The iterator to use.
 * @return The next thread or `null` if there is no next thread.
 */
thread_t* thread_iterator_next(thread_it* it);

/**
 * @internal
 * Initializes the threading subsystem.
 */
void thread_init();

void thread_save(thread_data* data);
void thread_enter(thread_data* data);

#endif /* THREADS_H_ */

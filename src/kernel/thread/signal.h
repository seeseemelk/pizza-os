/**
 * @file
 * A signal is a class that can be used to make a thread wait on an
 * external signal, such as an interrupt.
 * A thread can wait for the signal, which will cause the thread to
 * be set into a sleeping state, unless the signal is already set.
 * When something signals the thread, the waiting thread will be resumed.
 * @note Only one thread can be waiting on the signal at any one time.
 */

#ifndef THREAD_SIGNAL_H_
#define THREAD_SIGNAL_H_
#include "threads.h"
#include <stdbool.h>

/**
 * signal type definition.
 */
typedef struct signal_t signal_t;

struct signal_t
{
	thread_t* owner;
	bool signal;
};

/**
 * Creates a signal.
 * @param signal The signal to create.
 */
void signal_new(signal_t* signal);

/**
 * Unsets the signal the thread.
 * This can be used to discord a call to signal.
 * @param signal The signal to unset.
 */
void signal_clear(signal_t* signal);

/**
 * Waits until the signal is signaled.
 * This will put the current thread into sleep mode.
 * @param signal The signal to wait for.
 */
void signal_wait(signal_t* signal);

/**
 * Signals the signal.
 * This will take the owning thread out of sleep mode.
 */
void signal_signal(signal_t* signal);

/**
 * Checks if there is a thread waiting for this thread.
 * @param signal The signal to check for.
 * #returns `true` if a thread is waiting on the signal, `false` if no thread is waiting on it.
 */
bool signal_is_waiting(signal_t* signal);

/**
 * Checks if the thread is currently set.
 * @param signal The signal to check for.
 * @return `true` if the signal is set, `false` if it is cleared.
 */
bool signal_is_set(signal_t* signal);

#endif /* THREAD_SIGNAL_H_ */
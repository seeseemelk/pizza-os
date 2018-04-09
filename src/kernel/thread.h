/**
 * @file
 * Contains threading-related functions.
 */

#ifndef THREAD_H_
#define THREAD_H_
#include "cdefs.h"

/**
 * Contains information necessary to perform context switches.
 */
typedef struct thread_data thread_data;

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
	u32 eax;
	u32 ecx;
	u32 edx;
	u32 ebx;
	u32 esp;
	u32 ebp;
	u32 esi;
	u32 edi;
	void* stack;
} __attribute__((packed));

/**
 * Contains information about a thread.
 */
typedef struct thread_t thread_t;

/**
 * Represents a single thread.
 */
struct thread_t
{
	int id; /*< The unique ID of the thread to run. */
	void(*entry_point)(void); /*< The entry point of the thread. */
	thread_data data; /*< Contains important data required for performing context switches */
};

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
 * @internal
 * Initializes the threading subsystem.
 */
void thread_init();

#endif /* THREAD_H_ */

#include "thread/signal.h"
#include "threads.h"
#include "kernel.h"
#include "interrupt.h"

void signal_new(signal_t* signal)
{
	signal->owner = NULL;
	signal->signal = false;
}

void signal_clear(signal_t* signal)
{
	signal->owner = current_thread;
	signal->signal = false;
}

void signal_wait(signal_t* signal)
{
	interrupt_disable();
	signal->owner = current_thread;
	while (!signal->signal)
	{
		interrupt_enable();
		thread_set_paused(signal->owner, true);
		thread_leave();
		interrupt_disable();
	}
	signal->signal = false;
	signal->owner = NULL;
	interrupt_enable();
}

void signal_signal(signal_t* signal)
{
	interrupt_disable();
	if (signal->owner != NULL)
	{
		signal->signal = true;
		thread_set_paused(signal->owner, false);
	}
	interrupt_enable();
}

bool signal_is_set(signal_t* signal)
{
	return signal->signal;
}

bool signal_is_waiting(signal_t* signal)
{
	return signal->owner != NULL;
}

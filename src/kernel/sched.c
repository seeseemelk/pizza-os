/*
 * sched.c
 *
 *  Created on: Apr 10, 2018
 *      Author: seeseemelk
 */
#include "sched.h"
#include "threads.h"
#include "kernel.h"
#include "cpu.h"
#include <stdio.h>

void sched_main()
{
	thread_it it;
	while (1)
	{
		int alive_threads = 0;
		int running_threads = 0;
		thread_iterator_create(&it);
		thread_iterator_next(&it); // Skip the kernel thread.
		while (thread_iterator_has_next(&it))
		{
			thread_t* thread = thread_iterator_next(&it);
			if (thread->id != 0)
			{
				if (!thread_is_paused(thread))
				{
					thread_switch(thread);
					running_threads++;
				}
				alive_threads++;
			}
		}

		if (alive_threads == 0)
			kernel_panic("Last thread died - No one to eat my delicous pizza :(");
		else if (running_threads == 0)
			cpu_pause();
	}
}

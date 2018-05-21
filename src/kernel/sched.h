/**
 * @file
 * Contains main scheduling functions.
 *
 *  Created on: Apr 10, 2018
 *      Author: seeseemelk
 */

#ifndef SCHED_H_
#define SCHED_H_
#include "interrupt.h"

/**
 * Notifies the scheduler that it might want to preempt.
 * This is normally called by something like the PIT driver.
 * @param irq The IRQ that (indirectly) caused the call.
 * @param delay The time since the last call to sched_notify in us.
 */
void sched_notify(irq_t* irq, unsigned long long delay);

/**
 * Runs the main loop of the scheduler.
 */
void sched_main();

#endif /* SCHED_H_ */

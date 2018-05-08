/*
 * mutex.h
 *
 *  Created on: Apr 25, 2018
 *      Author: seeseemelk
 */

#ifndef THREAD_MUTEX_H_
#define THREAD_MUTEX_H_

#include "collections/list.h"
#include <stdbool.h>

typedef struct mutex_t mutex_t;
struct mutex_t
{
	list_t* waiting_threads;
	bool locked;
};

void mutex_new(mutex_t* mutex);
void mutex_free(mutex_t* mutex);
void mutex_lock(mutex_t* mutex);
void mutex_unlock(mutex_t* mutex);

#endif /* THREAD_MUTEX_H_ */

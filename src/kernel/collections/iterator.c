#include "iterator.h"
#include "kernel.h"
#include <stddef.h>
#include <stdlib.h>

it_t* it_new(has_next_fn* has_next, next_fn* next)
{
	it_t* it = malloc(sizeof(it));
	it->has_next = has_next;
	it->next = next;
	it->free = NULL;
	return it;
}

void it_free(it_t* it)
{
	if (it->free != NULL)
		free(it->data);
	free(it);
}

bool it_has_next(it_t* it)
{
	return it->has_next(it);
}

void* it_next(it_t* it)
{
	if (it_has_next(it))
		return it->next(it);
	else
		kernel_panic("Reached end of iterator");
	return NULL;
}

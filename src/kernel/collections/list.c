#include "collections/list.h"
#include "kernel.h"
#include <stdlib.h>

list_t* list_new()
{
	return list_new_sized(LIST_DEFAULT_SIZE);
}

list_t* list_new_sized(size_t size)
{
	list_t* list = malloc(sizeof(list_t));
	list->size = 0;
	list->max_size = size;
	list->contents = malloc(sizeof(void*) * list->max_size);
	return list;
}

void list_free(list_t* list)
{
	free(list->contents);
	free(list);
}

size_t list_size(list_t* list)
{
	return list->size;
}

void* list_get(list_t* list, size_t index)
{
	if (index < list->size)
		return list->contents[index];
	else
		kernel_panic("Tried to read outside of list.");
	return NULL;
}

void list_set(list_t* list, size_t index, void* item)
{
	if (index < list->size)
		list->contents[index] = item;
	else
		kernel_panic("Tried to write outside of list.");
}

void list_add(list_t* list, void* item)
{
	if (list->size == list->max_size)
	{
		list->max_size *= 2;
		list->contents = realloc(list->contents, sizeof(void*) * list->max_size);
	}

	list->contents[list->size] = item;
	list->size++;
}

void* list_pop(list_t* list)
{
	if (list->size > 0)
	{
		list->size--;
		return list->contents + list->size;
	}
	else
		kernel_panic("Tried to pop from empty list.");
	return NULL;
}

void list_insert(list_t* list, size_t index, void* item)
{
	if (index < list->size)
	{
		for (size_t i = list->size; i > index; i--)
			list->contents[i] = list->contents[i-1];
		list->contents[index] = item;
		list->size++;
	}
	else
	{
		kernel_panic("Tried to insert item beyond size of list.");
	}
}

void* list_remove(list_t* list, size_t index)
{
	if (index < list->size)
	{
		void* item = list->contents + index;
		list->size--;
		for (size_t i = index; i < list->size; i++)
			list->contents[i] = list->contents[i+1];
		return item;
	}
	else
	{
		kernel_panic("Tried to remove item beyond size of list.");
		return NULL;
	}
}


















/**
 * @file
 * List is a simple implementation of dynamic arrays.
 * All List functions are prefixed with 'list_'.
 */

#ifndef COLLECTIONS_LIST_H_
#define COLLECTIONS_LIST_H_

#include <stddef.h>

#define LIST_DEFAULT_SIZE 8

typedef struct list_t
{
	size_t size;
	size_t max_size;
	void** contents;
} list_t;

/**
 * Creates a new, empty list starting with a default size.
 * @return A newly created list.
 */
list_t* list_new(void);

/**
 * Creates a new, empty list starting with a given size.
 * @param size The size to give to the list.
 * @return A newly created list.
 */
list_t* list_new_sized(size_t size);

/**
 * Frees a list.
 * @param The list to free.
 */
void list_free(list_t* list);

/**
 * Gets the current size of the list.
 * @param list The list to get the size of.
 * @return The size of the given list.
 */
size_t list_size(list_t* list);

/**
 * Gets an item at a specific index.
 * @param list The list to get it from.
 * @param index The index to get it from.
 * @return The item at the specific index.
 */
void* list_get(list_t* list, size_t index);

/**
 * Sets an item at a specific index.
 * @param list The list to set an item in.
 * @param index The index to set the item at.
 * @param item The item to save in the list.
 */
void list_set(list_t* list, size_t index, void* item);

/**
 * Adds an item at the end of the list.
 * @param list The list to add the item to.
 * @param item The item to add.
 */
void list_add(list_t* list, void* item);

/**
 * Removes the item at the top of the list.
 * @param list The list to remove the item from.
 * @return The item that was removed.
 */
void* list_pop(list_t* list);

/**
 * Inserts an item into a left.
 * Shifts all items at equal-or-higher indexes one index up.
 * @param list The list to add the item to.
 * @param index The index to add the item at.
 * @param item The item to add.
 */
void list_insert(list_t* list, size_t index, void* item);

/**
 * Removes an item from an index.
 * Shifts all items at higher indexes one index down.
 * @param list The list to remove the item from.
 * @param index The index to remove the item from.
 * @return The removed item.
 */
void* list_remove(list_t* list, size_t index);

#endif /* COLLECTIONS_LIST_H_ */
















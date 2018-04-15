/**
 * A simple iterator type.
 * It can be allocated with `it_new`.
 * An iterator should implement the `next` and `has_next` functions.
 */

#ifndef COLLECTIONS_ITERATOR_H_
#define COLLECTIONS_ITERATOR_H_

#include <stdbool.h>

typedef struct it_t it_t;

typedef bool(has_next_fn)(it_t*);
typedef void*(next_fn)(it_t*);
typedef void(free_fn)(it_t*);

/**
 * The iterator type.
 */
struct it_t
{
	has_next_fn* has_next; /*< A function that returns whether or not there are more elements */
	next_fn* next; /*< A function that returns the next element if there is one. < */
	free_fn* free; /*< A function that is called when the iterator is to be cleaned up. Is only called if not null. */
	void* data; /*< Contains a pointer to data possibly required by the function implementor.*/
};

/**
 * Creates a new iterator.
 * @param has_next The `has_next` function to use.
 * @param next The `next` function to use.
 * @return A newly allocated iterator.
 */
it_t* it_new(has_next_fn* has_next, next_fn* next);

/**
 * Frees an iterator that was previously allocated with it_free.
 * @param it The iterator to free.
 */
void it_free(it_t* it);

/**
 * Checks if there is a next element in the iterator.
 * A return value of `true` means that a call to `it_next` is definitely possible.
 * @param The iterator to check.
 * @return `true` if there is a next element, `false` if there isn't.
 */
bool it_has_next(it_t* it);

/**
 * Gets the next element from this iterator.
 * @note If `it_has_next` returns false, the return value of this function is undefined.
 * @param it The iterator to check for.
 * @return A pointer to the next item.
 */
void* it_next(it_t* it);

#endif /* COLLECTIONS_ITERATOR_H_ */

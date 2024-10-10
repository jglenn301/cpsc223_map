#ifndef __SIMAP_H__
#define __SIMAP_H__

#include <stdbool.h>
#include <stddef.h>


typedef struct _simap simap;

extern char *simap_error;


/**
 * Creates an empty map that uses the given hash function.
 *
 * @param h a pointer to a function that takes a string and returns
 * its hash code, non-NULL
 * @return a pointer to the new map or NULL if it could not be created;
 * it is the caller's responsibility to destroy the map
 */
simap *simap_create(size_t (*h)(const char *s));


/**
 * Returns the number of (key, value) pairs in the given map.
 *
 * @param m a pointer to a map, non-NULL
 * @return the size of m
 */
size_t simap_size(const simap *m);


/**
 * Adds a copy of the given key with value to this map.
 * If the key is already present then the old value is replaced.
 * The caller retains ownership of the value.  If key is new
 * and space could not be allocated then there is no effect on the map
 * and the return value is simap_error.
 *
 * @param m a pointer to a map, non-NULL
 * @param key a string, non-NULL
 * @param value a pointer
 * @return a pointer to the old value, or NULL
 */
void simap_put(simap *m, const char *key, int value);


/**
 * Determines if the given key is present in this map.
 *
 * @param m a pointer to a map, non-NULL
 * @param key a string, non-NULL
 * @return true if key is present in this map, false otherwise
 */
bool simap_contains_key(const simap *m, const char *key);


/**
 * Returns the value associated with the given key in this map.
 * If the key is not present in this map then the returned value is
 * NULL.  The value returned is the original value passed to simap_put,
 * and it remains the responsibility of whatever called simap_put to
 * release the value (no ownership transfer results from simap_get).
 *
 * @param m a pointer to a map, non-NULL
 * @param key a string, non-NULL
 * @return the assocated value, or NULL if they key is not present
 */
int simap_get(simap *m, const char *key);


/**
 * Removes the given key and its associated value from the given map if
 * the key is present.  The return value is NULL and there is no effect
 * on the map if the key is not present.
 *
 * @param m a pointer to a map, non-NULL
 * @param key a key, non-NULL
 * @return the value associated with the key, or NULL
 */
void simap_remove(simap *m, const char *key);


/**
 * Calls the given function for each (key, value) pair in this map, passing
 * the extra argument as well.  This function does not add or remove from
 * the map.
 *
 * @param m a pointer to a map, non-NULL
 * @param f a pointer to a function that takes a key, a value, and an
 * extra argument, and does not add to or remove from the map, no matter
 * what the extra argument is; non-NULL
 * @param arg a pointer
 */
void simap_for_each(simap *m, void (*f)(const char *, int, void *), void *arg);


/**
 * Returns a dynamically array containing pointers to the keys in the
 * given map.  It is the caller's responsibility to free the array,
 * but the map retains ownership of the keys.  If there is a memory
 * allocation error then the returned value is NULL.  If the map is empty
 * then the returned value is NULL.
 *
 * @param m a pointer to a map, non NULL
 * @return a pointer to a dynamically allocated array of pointer to the keys
 */
const char **simap_keys(simap *m);


/**
 * Destroys the given map.
 *
 * @param m a pointer to a map, non-NULL
 */
void simap_destroy(simap *m);

#endif

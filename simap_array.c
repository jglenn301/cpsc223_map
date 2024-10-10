#include "simap.h"

#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

char *simap_error = "error";

/**
 * An implementation of a map of strings as an unsorted fixed-size array.
 * This implementation will not meet the requirements of Assignment #4 and
 * not completely tested itself but should be sufficient for testing your
 * Blotto competition program.
 */

typedef struct
{
  char *key;
  int value;
} entry;

struct _simap
{
  size_t capacity;
  size_t size;
  size_t (*hash)(const char *);
  entry *table;
};

#define SIMAP_INITIAL_CAPACITY 100

size_t simap_table_find_key(const entry *table, const char *key, size_t (*hash)(const char *), size_t size, size_t capacity);
void simap_embiggen(simap *m);

simap *simap_create(size_t (*h)(const char *s))
{
  // this implementation doesn't need the hash function so we skip the
  // check that it is not NULL
  
  simap *result = malloc(sizeof(simap));
  if (result != NULL)
    {
      result->size = 0;
      result->hash = h;
      result->table = malloc(sizeof(entry) * SIMAP_INITIAL_CAPACITY);
      result->capacity = (result->table != NULL ? SIMAP_INITIAL_CAPACITY : 0);
    }
  return result;
}

size_t simap_size(const simap *m)
{
  if (m == NULL)
    {
      return 0;
    }
  
  return m->size;
}

/**
 * Returns the index where key is located in the given map, or the index
 * where it would go if it is not present.
 * 
 * @param table a table with at least one free slot
 * @param key a string, non-NULL
 * @param hash a hash function for strings
 * @param size the size of the table
 * @param capacity the capacity of the table
 * @return the index of key in table, or the index of the empty slot to put it in if it is not present
 */
size_t simap_table_find_key(const entry *table, const char *key, size_t (*hash)(const char *), size_t size, size_t capacity)
{
  // sequential search for key
  size_t i = 0;
  while (i < size && strcmp(table[i].key, key) != 0)
    {
      i++;
    }
  return i;
}

void simap_put(simap *m, const char *key, int value)
{
  if (m != NULL && key != NULL)
    {
      if (m->size == m->capacity)
	{
	  // table is full (would that be the right condition for a hash table?); try to expand
	  simap_embiggen(m); // does nothing in this implementation
	}

      // find key
      int index = simap_table_find_key(m->table, key, m->hash, m->size, m->capacity);
      if (index < m->size)
	{
	  // key already present
	  m->table[index].value = value;
	}
      else
	{
	  // new key; check whether there is room for the new entry
	  if (m->size == m->capacity)
	    {
	      // still no room :(
	      return;
	    }
	  
	  // make a copy of the new key
	  char *copy = malloc(strlen(key) + 1);
	  strcpy(copy, key);
	  
	  if (copy != NULL)
	    {
	      m->table[index].key = copy;
	      m->table[index].value = value;
	      m->size++;
	      return;
	    }
	  else
	    {
	      return;
	    }
	}
    }
  else
    {
      // NULL matrix or key
      return;
    }
}

void simap_remove(simap *m, const char *key)
{
  // TO-DO: write and test remove
}

void simap_embiggen(simap *m)
{
  // TO-DO: write and test embiggen
}

bool simap_contains_key(const simap *m, const char *key)
{
  return simap_table_find_key(m->table, key, m->hash, m->size, m->capacity) < m->size;
}

int simap_get(simap *m, const char *key)
{
  int index = simap_table_find_key(m->table, key, m->hash, m->size, m->capacity);
  return m->table[index].value;
}


/**
 * A location in an array where a key can be stored.  The location is
 * represented by a (array, index) pair.
 */
typedef struct _simap_store_location
{
  const char **arr;
  size_t index;
} simap_store_location;



/**
 * Stores the given key in the given location and increments the location
 * by one element.
 *
 * @param key a pointer to a key, non-NULL
 * @param value ignored
 * @param arg a pointer to a simap_store_location
 */
void simap_store_key_in_array(const char *key, int value, void *arg)
{
  simap_store_location *where = arg;
  where->arr[where->index] = key;
  where->index++;
}


const char **simap_keys(simap *m)
{
  const char **keys = malloc(sizeof(*keys) * m->size);

  if (keys != NULL)
    {
      simap_store_location loc = {keys, 0};
      simap_for_each(m, simap_store_key_in_array, &loc);
    }

  return keys;
}


void simap_for_each(simap *m, void (*f)(const char *, int, void *), void *arg)
{
  if (m == NULL || f == NULL)
    {
      return;
    }
  
  for (int i = 0; i < m->size; i++)
    {
      f(m->table[i].key, m->table[i].value, arg);
    }
}  

void simap_destroy(simap *m)
{
  if (m == NULL)
    {
      return;
    }
  
  // free all the copies of the keys we made
  for (int i = 0; i < m->size; i++)
    {
      free(m->table[i].key);
    }

  // free the table
  free(m->table);

  // free the simap struct
  free(m);
}

int simap_default_hash(const char *s)
{
  // don't use this -- it is a terrible hash function!
  if (s == NULL)
    {
      return 0;
    }
  else
    {
      return s[0];
    }
}


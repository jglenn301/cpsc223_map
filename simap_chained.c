#include "simap.h"

#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>

/**
 * Nodes to implement chains as singly-linked lists.
 */
typedef struct _simap_node
{
  char *key;
  int value;
  struct _simap_node *next;
} simap_node;


/**
 * Meta-data struct for nodes.
 */
struct _simap
{
  simap_node **table;        // an array of pointers to heads of chains
  size_t size;         // the number of (key, value) pairs in the map
  size_t num_chains;   // the number of chains in the hash table
  
  size_t (*hash)(const char *);
};


#define SIMAP_INITIAL_CAPACITY 100


/**
 * Compute the index of the slot in which the given key would be
 * found in the given hash table.
 *
 * @param key a pointer to a string, non-NULL
 * @param hash a pointer to a hash function for strings, non-NULL
 * @param num_chains the number of chains in the hash table
 */
size_t simap_compute_index(const char *key, size_t (*hash)(const char *), size_t num_chains);


/**
 * Resizes the hash table in the given map to have at least the given number
 * of slots.  The current implementation does nothing.
 *
 * @param m a pointer to a map, non-NULL
 * @param n a positive integer
 */
void simap_embiggen(simap *m, size_t n);


/**
 * Adds the given node to the given hash table using the given hash function.
 *
 * @param table a pointer to an array of num_chains heads of chains, non-NULL
 * @param n a pointer to a node containing a non-NULL key and a value, non-NULL
 * @param hash a pointer to a hash function for strings, non-NULL
 * @param num_chains the number of chains in the hash table
 */
void simap_table_add(simap_node **table, simap_node *n, size_t (*hash)(const char *), size_t num_chains);


/**
 * Returns the node in the given chained hash table containing the given
 * key, or NULL if there is no such node.
 *
 * @param table a pointer to an array of num_chains heads of chains, non-NULL
 * @param key a pointer to a string, non-NULL
 * @param hash a pointer to a hash function for strings, non-NULL
 * @param num_chains the number of chains in the hash table
 * @return a pointer to a node containing key, or NULL
 */
simap_node *simap_table_find_key(simap_node **table, const char *key, size_t (*hash)(const char *), size_t num_chains);


simap *simap_create(size_t (*h)(const char *s))
{
  simap *result = malloc(sizeof(simap));
  if (result != NULL)
    {
      result->size = 0;
      result->hash = h;
      result->table = malloc(sizeof(simap_node *) * SIMAP_INITIAL_CAPACITY);
      result->num_chains = (result->table != NULL ? SIMAP_INITIAL_CAPACITY : 0);
      for (size_t i = 0; i < result->num_chains; i++)
	{
	  // all chains are initially empty
	  result->table[i] = NULL;
	}
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


simap_node *simap_table_find_key(simap_node **table, const char *key, size_t (*hash)(const char *), size_t num_chains)
{
  // determine which chain to search from hash function
  size_t i = simap_compute_index(key, hash, num_chains);

  // sequential search of the chain
  simap_node *curr = table[i];
  while (curr != NULL && strcmp(curr->key, key) != 0)
    {
      curr = curr->next;
    }
  return curr;
}


void simap_put(simap *m, const char *key, int value)
{
  simap_node *n = simap_table_find_key(m->table, key, m->hash, m->num_chains);
  if (n != NULL)
    {
      // key already present
      n->value = value;
    }
  else
    {
      // make space for a copy of the key
      char *copy = malloc(strlen(key) + 1);
      
      // copy contents of key
      strcpy(copy, key);
      
      // check if load factor is too high
      if (m->size >= m->num_chains)
	{
	  // add chains and rehash
	  simap_embiggen(m, m->num_chains * 2);
	}
	      
      // add to table
      simap_node *n = malloc(sizeof(simap_node));
      n->key = copy;
      n->value = value;
      simap_table_add(m->table, n, m->hash, m->num_chains);
      m->size++;
    }
}


void simap_table_add(simap_node **table, simap_node *n, size_t (*hash)(const char *), size_t num_chains)
{
  // get index of chain to add in
  size_t i = simap_compute_index(n->key, hash, num_chains);

  // add to front of chain
  n->next = table[i];
  table[i] = n;
}


void simap_embiggen(simap *m, size_t n)
{
  // this implementation does nothing; load factor will grow without bound!
  // NOOOOOOoooooooo! [quiet sobbing]
}


size_t simap_compute_index(const char *key, size_t (*hash)(const char *), size_t num_chains)
{
  // hash and take remainder
  return hash(key) % num_chains;
}


bool simap_contains_key(const simap *m, const char *key)
{
  return simap_table_find_key(m->table, key, m->hash, m->num_chains) != NULL;
}


int simap_get(simap *m, const char *key)
{
  // find the node containing the key
  simap_node *n = simap_table_find_key(m->table, key, m->hash, m->num_chains);
  return n->value;
}


void simap_remove(simap *m, const char *key)
{
  // remove not implemented
  return;
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

  for (size_t chain = 0; chain < m->num_chains; chain++)
    {
      simap_node *curr = m->table[chain];
      while (curr != NULL)
	{
	  f(curr->key, curr->value, arg);
	  curr = curr->next;
	}
    }
}  


void simap_destroy(simap *m)
{
  if (m == NULL)
    {
      return;
    }

  // iterate over all chains
  for (size_t i = 0; i < m->num_chains; i++)
    {
      // iterate over all nodes in chain
      simap_node *curr = m->table[i];
      while (curr != NULL)
	{
	  // free key
	  free(curr->key);

	  // remember where to go next
	  simap_node *next = curr->next;

	  // free node
	  free(curr);

	  // go to next node
	  curr = next;
	}
    }

  free(m->table);
  free(m);
}

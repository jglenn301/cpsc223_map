#include <string.h>

#include "string_key.h"

size_t hash29(const char *key)
{
  const char *s = key;
  size_t sum = 0;
  size_t factor = 29;
  while (s != NULL && *s != '\0')
    {
      sum += *s * factor;
      s++;
      factor *= 29;
    }

  return sum;
}

char *duplicate(const char *key)
{
  char *s = malloc(strlen(key) + 1);
  if (s != NULL)
    {
      strcpy(s, key);
    }
  return s;
}

int compare_keys(const char *key1, const char *key2)
{
  return strcmp(key1, key2);
}

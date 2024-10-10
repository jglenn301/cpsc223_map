#include <stdio.h>
#include <stdlib.h>

#include "simap.h"
#include "string_util.h"

void print_count_to_file(const char *key, int value, void *arg);


void print_count(const char *key, int value, void *arg);

int main(int argc, char **argv)
{
  const int MAX_BRAND_LEN = 70;
  char brand[MAX_BRAND_LEN];

  simap *counts = simap_create(NULL);
  
  read_line(brand, MAX_BRAND_LEN);
  while (!feof(stdin) && brand[0] != '\0')
    {
      // update counts for one more occurrence of brand
      if (!simap_contains_key(counts, brand))
	{
	  simap_put(counts, brand, 1);
	}
      else
	{
	  simap_put(counts, brand, simap_get(counts, brand) + 1);
	}
      
      read_line(brand, MAX_BRAND_LEN);
    }

  FILE *outfile = fopen("output", "w");
  
  simap_for_each(counts, print_count_to_file, outfile);
  
  simap_destroy(counts);
 
  return 0;
}

void print_count_to_file(const char *key, int value, void *arg)
{
  FILE *outfile = arg;
  fprintf(outfile, "%s = %d\n", key, value);
}

void print_count(const char *key, int value, void *arg)
{
  printf("%s = %d\n", key, value);
}

void print_count_to_error(const char *key, int value, void *arg)
{
  fprintf(stderr, "%s = %d\n", key, value);
}



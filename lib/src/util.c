#include "../include/util.h"
#include <stdio.h>
#include <stdlib.h>

void *_malloc(size_t size) {
  void *ptr = malloc(size);
  if (ptr == NULL) {
    perror("Error in malloc");
    exit(1);
  }
  return ptr;
}

void _free(void *ptr) {
  if (ptr == NULL) {
    perror("Error: Trying to free null ptr");
    exit(1);
  }
  free(ptr);
}

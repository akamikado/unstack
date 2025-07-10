#include "../include/util.h"
#include <stdio.h>
#include <stdlib.h>

void _perror(const char *err) {
  perror(err);
  exit(1);
}

void *_malloc(size_t size) {
  void *ptr = malloc(size);
  if (ptr == NULL) {
    _perror("Error in malloc");
  }
  return ptr;
}

void _free(void *ptr) {
  if (ptr == NULL) {
    _perror("Error: Trying to free null ptr");
  }
  free(ptr);
}

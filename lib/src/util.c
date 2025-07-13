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

int verify_checksum(void *hdr, u8 len) {
  u32 sum = 0;
  int pos = 0;
  for (; pos < len; pos += 2) {
    sum += *(u16 *)(hdr + pos);
  }
  sum = (sum >> 16) + (sum & 0xffff);
  sum = ~sum;
  return (sum & 0xffff) == 0;
}

u16 calculate_checksum(void *hdr, u8 len) {
  u32 sum = 0;
  int pos = 0;
  for (; pos < len; pos += 2) {
    sum += *(u16 *)(hdr + pos);
  }
  sum = (sum >> 16) + (sum & 0xffff);
  sum = ~sum;
  return (sum & 0xffff);
}

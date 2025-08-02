#ifndef UTIL_H
#define UTIL_H

#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

#define u8 uint8_t
#define u16 uint16_t
#define u32 uint32_t
#define u64 uint64_t

#ifdef DEBUG
#define debug(...)                                                             \
  do {                                                                         \
    printf(__VA_ARGS__);                                                       \
  } while (0)
#else
#define debug(...)                                                             \
  do {                                                                         \
  } while (0)
#endif

void *_malloc(size_t size);
void _free(void *ptr);
void _perror(const char *err);

u32 checksum_partial(void *hdr, u32 len);
u16 checksum_fold(u32 psum);
int verify_checksum(void *hdr, u32 len);
u16 calculate_checksum(void *hdr, u32 len);

u16 htons(u16 hostshort);
u32 htonl(u32 hostlong);

#endif

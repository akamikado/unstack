#ifndef UTIL_H
#define UTIL_H

#include <stdint.h>
#include <unistd.h>

#define u8 uint8_t
#define u16 uint16_t
#define u32 uint32_t

void *_malloc(size_t size);
void _free(void *ptr);
void _perror(const char *err);

int verify_checksum(void *hdr, u8 len);

#endif

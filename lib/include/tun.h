#ifndef TUN_H
#define TUN_H
#include <stdint.h>
#include <unistd.h>

int tun_allocate(char *dev);
size_t tun_read(int fd, uint8_t *buf);
void tun_write(int fd, uint8_t *buf, size_t count);

#endif

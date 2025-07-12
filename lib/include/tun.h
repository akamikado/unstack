#ifndef TUN_H
#define TUN_H
#include "util.h"
#include <unistd.h>

int tun_allocate(char *dev);
size_t tun_read(int fd, u8 *buf);
void tun_write(int fd, u8 *buf, size_t count);

#endif

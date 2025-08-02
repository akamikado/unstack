#ifndef TUN_H
#define TUN_H
#include "util.h"
#include <unistd.h>

extern int *tun_fd;

int tun_allocate(char *dev);
size_t tun_read(int fd, u8 *buf);
int tun_write(int fd, u8 *buf, size_t count);

#endif

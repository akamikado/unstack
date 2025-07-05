#ifndef TUN_H
#define TUN_H
#include <unistd.h>

int tun_allocate(char *dev);
size_t tun_read(int fd, char *buf);
void tun_write(int fd, char *buf, size_t count);

#endif

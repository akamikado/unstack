#include "../include/tun.h"
#include <fcntl.h>
#include <linux/if.h>
#include <linux/if_tun.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>

int tun_allocate(char *dev) {
  struct ifreq ifr;
  int fd, err;

  if ((fd = open("/dev/net/tun", O_RDWR)) < 0) {
    perror("error in opening /dev/net/tun");
    exit(1);
  }

  memset(&ifr, 0, sizeof(ifr));

  ifr.ifr_flags = IFF_TUN | IFF_NO_PI;

  if (*dev) {
    strncpy(ifr.ifr_name, dev, IFNAMSIZ);
  }

  if ((err = ioctl(fd, TUNSETIFF, (void *)&ifr)) < 0) {
    close(fd);
    perror("error in creating tun interface");
    exit(1);
  }

  strcpy(dev, ifr.ifr_name);
  return fd;
}

size_t tun_read(int fd, char *buf) {
  size_t len = read(fd, (void *)buf, UINT16_MAX);
  return len;
}

void tun_write(int fd, char *buf, size_t count) {
  write(fd, (void *)buf, count);
}

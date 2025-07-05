#ifndef NETDEV_H
#define NETDEV_H
#include <stdint.h>
#include <unistd.h>

struct net_device {
  uint32_t ipaddr;
};

void net_device_init();
struct net_device *net_device_alloc(const char *ipaddr);
void net_device_free();

#endif

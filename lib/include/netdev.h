#ifndef NETDEV_H
#define NETDEV_H
#include "util.h"
#include <unistd.h>

struct net_device {
  u32 ipaddr;
};

void net_device_init();
struct net_device *net_device_alloc(const char *ipaddr);
void net_device_free();

void net_device_start_loop();

#endif

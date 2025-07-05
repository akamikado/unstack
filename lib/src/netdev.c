#include "../include/netdev.h"
#include "../include/lib.h"
#include <stdio.h>

struct net_device *dev;

struct net_device *net_device_alloc(const char *ipaddr) {
  struct net_device *temp_dev = _malloc(sizeof(struct net_device));

  int octets[4];
  sscanf(ipaddr, "%d.%d.%d.%d", &octets[0], &octets[1], &octets[2], &octets[3]);
  temp_dev->ipaddr =
      octets[0] << 24 | octets[1] << 16 | octets[2] << 8 | octets[3];

  return temp_dev;
}

// Change ip address later
void net_device_init() { dev = net_device_alloc("120.0.0.1"); }

void net_device_free() { _free(dev); }

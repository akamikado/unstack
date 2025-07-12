#include "../include/netdev.h"
#include "../include/ip.h"
#include "../include/skb.h"
#include "../include/tun.h"
#include "../include/util.h"
#include <stdint.h>
#include <stdio.h>
#include <sys/poll.h>

struct net_device *dev;

struct net_device *net_device_alloc(const char *ipaddr) {
  struct net_device *temp_dev = _malloc(sizeof(struct net_device));

  int octets[4];
  sscanf(ipaddr, "%d.%d.%d.%d", &octets[0], &octets[1], &octets[2], &octets[3]);
  temp_dev->ipaddr =
      octets[0] << 24 | octets[1] << 16 | octets[2] << 8 | octets[3];

  return temp_dev;
}

// TODO: Change ip address later
void net_device_init() { dev = net_device_alloc("120.0.0.1"); }

void net_device_free() { _free(dev); }

static void net_device_receive_skb(int fd) {
  struct sk_buff *buff = alloc_skb();
  int len = tun_read(fd, buff->head);
  if (len == -1) {
    _perror("Error in reading from tun if");
  }
  buff->len = len;
  buff->tail = buff->head + buff->len;
  ip_rcv(buff);
}

void net_device_start_loop() {
  char tun_dev[UINT16_MAX];
  int tun_fd = tun_allocate(tun_dev);
  struct pollfd pfd = {.fd = tun_fd, .events = POLLIN};

  while (poll(&pfd, 1, -1) != -1) {
    if (pfd.revents & POLLIN) {
      net_device_receive_skb(tun_fd);
    }
  }
}

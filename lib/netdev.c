#include "include/netdev.h"
#include "include/ip.h"
#include "include/netlink.h"
#include "include/skb.h"
#include "include/tun.h"
#include "include/util.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
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

void net_device_start_loop(void *arg) {
  char *address = (char *)arg;
  u8 prefix_bits;
  if (split_address(address, &prefix_bits) == -1) {
    fprintf(stderr, "Invalid address \"%s\"\n", address);
    exit(1);
  }

  char tun_dev[UINT16_MAX];
  tun_fd = malloc(sizeof(int));
  *tun_fd = tun_allocate(tun_dev);

  int netlink_fd = netlink_connect();
  if (netlink_fd == -1) {
    _perror("netlink_connect");
    exit(1);
  }
  if (netlink_set_addr_ipv4(netlink_fd, tun_dev, address, prefix_bits) == -1) {
    perror("netlink_set_addr_ipv4");
    exit(1);
  }
  if (netlink_link_up(netlink_fd, tun_dev) == -1) {
    perror("netlink_link_up");
    exit(1);
  }
  close(netlink_fd);

  struct pollfd pfd = {.fd = *tun_fd, .events = POLLIN};

  while (poll(&pfd, 1, -1) != -1) {
    if (pfd.revents & POLLIN) {
      net_device_receive_skb(*tun_fd);
    }
  }
}

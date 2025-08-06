#ifndef NETLINK_H_
#define NETLINK_H_

#include "util.h"

int netlink_connect();
int netlink_set_addr_ipv4(int netlink_fd, const char *iface_name,
                          const char *address, u8 network_prefix_bits);
int netlink_link_up(int netlink_fd, const char *iface_name);
int split_address(char *address_str, uint8_t *network_prefix_bits);

#endif // NETLINK_H_

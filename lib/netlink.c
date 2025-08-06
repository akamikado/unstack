#include "include/netlink.h"
#include <arpa/inet.h>
#include <errno.h>
#include <linux/netlink.h>
#include <linux/rtnetlink.h>
#include <net/if.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int netlink_connect() {
  int netlink_fd, rc;
  struct sockaddr_nl sockaddr;

  netlink_fd = socket(AF_NETLINK, SOCK_RAW | SOCK_CLOEXEC, NETLINK_ROUTE);
  if (netlink_fd == -1) {
    return -1;
  }

  memset(&sockaddr, 0, sizeof sockaddr);
  sockaddr.nl_family = AF_NETLINK;
  rc = bind(netlink_fd, (struct sockaddr *)&sockaddr, sizeof sockaddr);
  if (rc == -1) {
    int bind_errno = errno;
    close(netlink_fd);
    errno = bind_errno;
    return -1;
  }
  return netlink_fd;
}

int netlink_set_addr_ipv4(int netlink_fd, const char *iface_name,
                          const char *address, u8 network_prefix_bits) {
  struct {
    struct nlmsghdr header;
    struct ifaddrmsg content;
    char attributes_buf[64];
  } request;

  struct rtattr *request_attr;
  size_t attributes_buf_avail = sizeof request.attributes_buf;

  memset(&request, 0, sizeof request);
  request.header.nlmsg_len = NLMSG_LENGTH(sizeof request.content);
  request.header.nlmsg_flags = NLM_F_REQUEST | NLM_F_EXCL | NLM_F_CREATE;
  request.header.nlmsg_type = RTM_NEWADDR;
  request.content.ifa_index = if_nametoindex(iface_name);
  request.content.ifa_family = AF_INET;
  request.content.ifa_prefixlen = network_prefix_bits;

  /* request.attributes[IFA_LOCAL] = address */
  request_attr = IFA_RTA(&request.content);
  request_attr->rta_type = IFA_LOCAL;
  request_attr->rta_len = RTA_LENGTH(sizeof(struct in_addr));
  request.header.nlmsg_len += request_attr->rta_len;
  inet_pton(AF_INET, address, RTA_DATA(request_attr));

  /* request.attributes[IFA_ADDRESS] = address */
  request_attr = RTA_NEXT(request_attr, attributes_buf_avail);
  request_attr->rta_type = IFA_ADDRESS;
  request_attr->rta_len = RTA_LENGTH(sizeof(struct in_addr));
  request.header.nlmsg_len += request_attr->rta_len;
  inet_pton(AF_INET, address, RTA_DATA(request_attr));

  if (send(netlink_fd, &request, request.header.nlmsg_len, 0) == -1) {
    return -1;
  }
  return 0;
}

int netlink_link_up(int netlink_fd, const char *iface_name) {
  struct {
    struct nlmsghdr header;
    struct ifinfomsg content;
  } request;

  memset(&request, 0, sizeof request);
  request.header.nlmsg_len = NLMSG_LENGTH(sizeof request.content);
  request.header.nlmsg_flags = NLM_F_REQUEST;
  request.header.nlmsg_type = RTM_NEWLINK;
  request.content.ifi_index = if_nametoindex(iface_name);
  request.content.ifi_flags = IFF_UP;
  request.content.ifi_change = 1;

  if (send(netlink_fd, &request, request.header.nlmsg_len, 0) == -1) {
    return -1;
  }
  return 0;
}

int split_address(char *address_str, uint8_t *network_prefix_bits) {
  char *prefix_sep, *prefix_str;

  prefix_sep = strchr(address_str, '/');
  if (prefix_sep == NULL) {
    prefix_str = NULL;
    *network_prefix_bits = 32;
  } else {
    *prefix_sep = 0;
    prefix_str = prefix_sep + 1;
  }

  if (inet_addr(address_str) == INADDR_NONE) {
    return -1;
  }

  if (prefix_str != NULL) {
    char *prefix_extra;
    long prefix_raw = strtol(prefix_str, &prefix_extra, 10);

    if (prefix_raw < 0 || prefix_raw > 32) {
      *prefix_sep = '/';
      return -1;
    }
    if (*prefix_extra != 0) {
      *prefix_sep = '/';
      return -1;
    }
    *network_prefix_bits = prefix_raw;
  }

  return 0;
}

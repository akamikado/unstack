/*
 * TODO: Remove all hardcoded stuff
 * */

#include "include/inet.h"
#include "include/ip.h"
#include "include/tcp.h"

int inet_create(struct socket *sock, int protocol) {
  int retval = -1;
  if (protocol != 0 && protocol != IP_PROTO_TCP) {
    goto out;
  }
  struct sock *sk = tcp_alloc_sock();
  if (sk == NULL) {
    goto out;
  }
  sk->protocol = protocol;
  switch (protocol) {
  case 0:
  case IP_PROTO_TCP:
    sk->ops = &tcp_ops;
    break;
  default:
    goto error;
  }
  sock->sk = sk;
  retval = 0;
  goto out;

error:
  _free(sk);
out:
  return retval;
}

int inet_bind(struct socket *sock, struct sockaddr *addr) {
  int retval = -1;
  if (!addr || !addr->src_addr || !addr->src_port) {
    goto out;
  }
  struct sock *sk = sock->sk;
  sk->addr.src_addr = addr->src_addr;
  retval = sk->ops->set_port(sk, addr->src_port);
out:
  return retval;
}

int inet_connect(struct socket *sock, struct sockaddr *addr) {
  int retval = -1;
  if (!addr || !addr->dst_addr || !addr->dst_port) {
    goto out;
  }
  retval = sock->sk->ops->connect(sock->sk, addr);
out:
  return retval;
}

struct socket_ops inet_ops = {
    .create = inet_create,
    .bind = inet_bind,
    .connect = inet_connect,
};

int inet_pton(enum domain_type domain, const char *addr, void *dst) {
  int retval = -1;
  switch (domain) {
  case AF_INET:
    u32 octets[4] = {0};
    if (sscanf(addr, "%d.%d.%d.%d", &octets[0], &octets[1], &octets[2],
               &octets[3]) != 4) {
      goto out;
    }
    *(u32 *)dst = (u32)((octets[0] << 24) | (octets[1] << 16) |
                        (octets[2] << 8) | (octets[3]));
    retval = 1;
    goto out;
    break;
  default:
    goto out;
  }
out:
  return retval;
}

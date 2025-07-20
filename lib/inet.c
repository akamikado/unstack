/*
 * TODO: Remove all hardcoded stuff
 * */

#include "include/inet.h"
#include "include/ip.h"
#include "include/tcp.h"

int inet_create(struct socket *sock, int protocol) {
  int retval = -1;
  if (protocol != 0 || protocol != IP_PROTO_TCP) {
    goto out;
  }
  struct sock *sk = tcp_alloc_sock();
  sk->protocol = protocol;
  if (sk == NULL) {
    goto out;
  }
  sock->sk = sk;
  retval = 0;
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

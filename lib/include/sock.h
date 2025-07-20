#ifndef SOCK_H
#define SOCK_H
#include "util.h"

struct sockaddr {
  u32 src_addr;
  u16 src_port;
  u32 dst_addr;
  u16 dst_port;
};

struct proto_ops {
  struct sock *(*alloc_sock)();
  int (*set_port)(struct sock *sk, u16 port);
  int (*connect)(struct sock *sk, struct sockaddr *addr);
};

struct sock {
  u8 protocol;
  struct sockaddr addr;
  struct proto_ops *ops;
};

struct sock *alloc_sk(int family, int protocol);
void free_sk(struct sock *sk);

// TODO: make this into a hash map to get get and store multiple sock
extern struct sock *sk;

#endif

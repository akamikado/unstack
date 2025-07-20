#ifndef SOCKET_H
#define SOCKET_H
#include "sock.h"
#include "util.h"

enum socket_state {
  SS_FREE,
  SS_UNCONNECTED,
  SS_CONNECTING,
  SS_CONNECTED,
  SS_DISCONNECTED
};

enum domain_type { AF_INET = 2 };
enum socket_type { SOCK_STREAM = 1 };

struct socket {
  enum socket_state state;
  enum domain_type domain;
  enum socket_type type;
  struct socket_ops *ops;
  struct sock *sk;
};

struct socket_ops {
  int (*create)(struct socket *sock, int protocol);
  int (*bind)(struct socket *sock, struct sockaddr *addr);
  int (*connect)(struct socket *sock, struct sockaddr *addr);
  struct socket *(*accept)(struct socket *sock, struct sockaddr *addr);
  int (*listen)(struct socket *sock);
  int (*release)(struct socket *sock);
};

struct socket *_socket(int domain, int type, int protocol);
int _bind(struct socket *sock, struct sockaddr *addr);
int _connect(struct socket *sock, struct sockaddr *serv_addr);
struct socket *_accpet(struct socket *sock, struct sockaddr *addr);
int _listen(struct socket *sock);
int _close(struct socket *sock);

#endif

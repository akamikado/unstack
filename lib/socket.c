#include "include/socket.h"
#include "include/inet.h"

// TODO: change to file descriptor
struct socket *_socket(int domain, int type, int protocol) {
  struct socket *sock = NULL;
  sock = (struct socket *)_malloc(sizeof(struct socket));
  sock->state = SS_UNCONNECTED;
  sock->domain = domain;
  sock->type = type;
  sock->ops = &inet_ops;
  if (sock->ops->create(sock, protocol)) {
    goto error;
  }
  return sock;
error:
  _free(sock);
  return NULL;
}

// TODO
int _bind(struct socket *sock, struct sockaddr *addr) {
  int retval = -1;
  if (sock == NULL || addr == NULL) {
    goto out;
  }
out:
  return retval;
}

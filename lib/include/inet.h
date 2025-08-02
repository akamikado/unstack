#ifndef INET_H
#define INET_H
#include "socket.h"

extern struct socket_ops inet_ops;

int inet_pton(enum domain_type domain, const char *addr, void *dst);
#endif

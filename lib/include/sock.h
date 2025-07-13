#ifndef SOCK_H
#define SOCK_H

enum ProtocolFamily { AF_INET };

struct sock {
  enum ProtocolFamily family;

  // TODO: read and write sk_buff queues
};

#endif

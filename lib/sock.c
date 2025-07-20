#include "include/sock.h"
#include "include/util.h"

struct sock *sk;

struct sock *alloc_sk(int family, int protocol) {
  struct sock *sk = NULL;
  sk = (struct sock *)_malloc(sizeof(struct sock));
  sk->protocol = protocol;
out:
  return sk;
}

void free_sk(struct sock *sk) { _free(sk); }

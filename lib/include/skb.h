#ifndef SKB_H
#define SKB_H
#include "sock.h"
#include "util.h"

struct sk_buff {
  struct sock *sk;
  u32 len;
  u8 *head;
  u8 *data;
  u8 *tail;
  u8 *end;
};

struct sk_buff *alloc_skb();
void free_skb(struct sk_buff *skb);

#endif

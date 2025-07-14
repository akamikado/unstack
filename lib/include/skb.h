#ifndef SKB_H
#define SKB_H
#include "sock.h"
#include "util.h"

struct sk_buff {
  struct sock *sk;
  u8 cb[20];
  u32 len;
  u8 *head;
  u8 *data;
  u8 *tail;
  u8 *end;
};

struct sk_buff *alloc_skb();
void free_skb(struct sk_buff *skb);

static inline void skb_reserve(struct sk_buff *skb, int len) {
  skb->data += len;
  skb->tail += len;
}

static inline u8 *skb_put(struct sk_buff *skb, int len) {
  u8 *pos = skb->tail;
  skb->tail += len;
  skb->len += len;
  return pos;
}

static inline u8 *skb_push(struct sk_buff *skb, int len) {
  skb->data -= len;
  skb->len += len;
  return skb->data;
}

#endif

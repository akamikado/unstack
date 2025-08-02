#include "include/skb.h"
#include "include/util.h"

struct sk_buff *alloc_skb() {
  struct sk_buff *new = (struct sk_buff *)_malloc(sizeof(struct sk_buff));
  new->head = (u8 *)_malloc(UINT16_MAX);
  new->data = new->head;
  new->tail = new->head;
  new->end = new->head + UINT16_MAX;
  return new;
}

void free_skb(struct sk_buff *skb) {
  _free(skb->head);
  _free(skb);
}

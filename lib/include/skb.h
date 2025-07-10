#ifndef SKB_H
#define SKB_H
#include <stdint.h>

struct sk_buff {
  uint32_t len;
  uint8_t *head;
  uint8_t *data;
  uint8_t *tail;
  uint8_t *end;
};

struct sk_buff *alloc_skb();

#endif

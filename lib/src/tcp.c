#include "../include/tcp.h"

static struct tcphdr *get_header(struct sk_buff *skb) {
  return (struct tcphdr *)(skb->data);
}

static int verify_checksum(struct tcphdr *hdr, u8 len) {
  u32 sum = 0;
  size_t pos = 0;
  for (; pos < len; pos += 2) {
    sum += *(u16 *)(hdr + pos);
  }
  sum = (sum >> 16) + (sum & 0xffff);
  sum = ~sum;
  return (sum & 0xffff) == 0;
}

void tcp_rcv(struct sk_buff *skb) {
  struct tcphdr *hdr = get_header(skb);
  skb->data += hdr->data_offset * 4;
  if (!verify_checksum(hdr, hdr->data_offset)) {
    goto error;
  }

error:
drop:
  free_skb(skb);
out:
  return;
}

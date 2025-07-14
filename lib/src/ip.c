#include "../include/ip.h"
#include "../include/tcp.h"

static struct iphdr *get_hdr(struct sk_buff *skb) {
  return (struct iphdr *)(skb->data);
}

void ip_rcv(struct sk_buff *skb) {
  struct iphdr *hdr = (struct iphdr *)skb_pull(skb, get_hdr(skb)->ihl * 4);

  if (hdr->ihl < 5 || hdr->ver != 4) {
    goto error;
  }

  if (!verify_checksum(hdr, hdr->ihl)) {
    goto drop;
  }

  switch (hdr->protocol) {
  case PROTOCOL_TCP:
    tcp_rcv(skb);
  default:
    goto drop;
  }

error:
  // TODO
drop:
  free_skb(skb);
out:
  return;
}

void ip_transmit_skb(struct sk_buff *skb, struct sock *sk) {
  if (skb == NULL) {
    return;
  }
  struct ip_sock *isk = (struct ip_sock *)sk;
  struct iphdr *ih = (struct iphdr *)skb_push(skb, 20);
  // No options in ip header
  *(u16 *)ih = htons((4 << 12) | 5 << 8 | (isk->tos & 0xff));
  ih->len = htons(skb->len);
  ih->id = 0;
  *((u16 *)ih + 3) = htons((isk->flags & 0x07) << 13 | 0); // 0 is frag_offset
  ih->ttl = isk->ttl;
  ih->protocol = isk->protocol;
  ih->csum = 0;
  ih->src_addr = isk->src_addr;
  ih->dst_addr = isk->dst_addr;
  ih->csum = calculate_checksum(ih, 20);

  // TODO: figure out how to transmit skb
}

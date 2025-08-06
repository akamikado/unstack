#include "include/ip.h"
#include "include/tcp.h"
#include "include/tun.h"

static struct iphdr *get_hdr(struct sk_buff *skb) {
  return (struct iphdr *)(skb->data);
}

/*
 * TODO:
 * 1. fix error handling
 * */
void ip_rcv(struct sk_buff *skb) {
  struct iphdr *ih = (struct iphdr *)skb_pull(skb, get_hdr(skb)->ihl * 4);

  if (ih->ihl < 5 || ih->ver != 4) {
    goto error;
  }

  if (!verify_checksum(ih, ih->ihl)) {
    goto drop;
  }

  switch (ih->protocol) {
  case IP_PROTO_TCP:
    tcp_rcv(skb);
  default:
    goto drop;
  }

error:
drop:
  free_skb(skb);
out:
  return;
}

/*
 * TODO:
 * 1. Fix how the data is passed to lower layer
 * 2. Remove hardcoded ttl
 * */
int ip_transmit_skb(struct sk_buff *skb, struct sock *sk) {
  int retval = -1;
  if (skb == NULL) {
    goto out;
  }
  struct ip_sock *isk = (struct ip_sock *)sk;
  struct iphdr *ih = (struct iphdr *)skb_push(skb, 20);
  // No options in ip header
  *(u16 *)ih = htons((4 << 12) | 5 << 8 | (isk->tos & 0xff));
  ih->len = htons(skb->len);
  ih->id = 0;
  *((u16 *)ih + 3) = htons((isk->flags & 0x07) << 13 | 0); // 0 is frag_offset
  ih->ttl = 64;
  ih->protocol = isk->protocol;
  ih->csum = 0;
  ih->src_addr = htonl(isk->src_addr);
  ih->dst_addr = htonl(isk->dst_addr);
  ih->csum = htons(calculate_checksum(ih, 20));

  retval = tun_write(*tun_fd, skb->data, skb->len);
out:
  return retval;
}

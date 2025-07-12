#include "../include/ip.h"
#include "../include/tcp.h"

static struct iphdr *get_hdr(struct sk_buff *skb) {
  return (struct iphdr *)(skb->data);
}

static int verify_ip_csum(struct iphdr *hdr, u8 ihl) {
  u32 sum = 0;
  int pos = 0;
  for (; pos < ihl; pos += 2) {
    sum += *(u16 *)(hdr + pos);
  }
  sum = (sum >> 16) + (sum & 0xffff);
  sum = ~sum;
  return (sum & 0xffff) == 0;
}

void ip_rcv(struct sk_buff *skb) {
  struct iphdr *hdr = get_hdr(skb);
  skb->data += hdr->ihl * 4;

  if (hdr->ihl < 5 || hdr->ver != 4) {
    goto error;
  }

  if (!verify_ip_csum(hdr, hdr->ihl)) {
    goto drop;
  }

  switch ((enum ProtocolNumbers)hdr->protocol) {
  case TCP:
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

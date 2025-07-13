#include "../include/ip.h"
#include "../include/tcp.h"

static struct iphdr *get_hdr(struct sk_buff *skb) {
  return (struct iphdr *)(skb->data);
}

void ip_rcv(struct sk_buff *skb) {
  struct iphdr *hdr = get_hdr(skb);
  skb->data += hdr->ihl * 4;

  if (hdr->ihl < 5 || hdr->ver != 4) {
    goto error;
  }

  if (!verify_checksum(hdr, hdr->ihl)) {
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

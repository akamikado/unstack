#include "../include/tcp.h"

static struct tcphdr *get_header(struct sk_buff *skb) {
  return (struct tcphdr *)(skb->data);
}

void tcp_rcv(struct sk_buff *skb) {
  struct tcphdr *hdr = get_header(skb);
  skb->data += hdr->data_offset * 4;
  if (!verify_checksum(hdr, hdr->data_offset)) {
    goto error;
  }

  // TODO: verify packet and pass data up

error:
drop:
  free_skb(skb);
out:
  return;
}

void tcp_transmit_skb(struct sk_buff *skb, struct sock *sk) {
  if (skb == NULL) {
    return;
  }

  /*
   * TODO:
   * 1. create tcp header with seq and ack num using snd variables in sk
   * 2. move data header in skb back by size of tcp header and copy header data
   * to where data ptr is
   * 3. send to ip_transmit_skb
   * */
}

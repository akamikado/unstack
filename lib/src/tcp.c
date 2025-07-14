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

  struct tcp_sock *tsk = (struct tcp_sock *)sk;
  struct tcb_skb *tcb = TCP_SKB_TCB(skb);

  // Not supporting options
  int tcp_header_size = 20;
  struct tcphdr *th = (struct tcphdr *)skb_push(skb, MAX_TCPHDR_LEN);
  th->src_port = tsk->src_port;
  th->dst_port = tsk->dst_port;
  th->seq_num = tcb->seq;
  th->ack_num = tcb->ack;
  *((u8 *)th + 12) = htons((tcp_header_size >> 2) << 12 | tcb->flags);
  th->window = tsk->rcv_wnd;
  th->checksum = 0;
  th->urg_ptr = 0;
  th->checksum = calculate_checksum(th, tcp_header_size);

  ip_transmit_skb(skb, sk);
}

#include "../include/tcp.h"

static u16 calculate_tcp_checksum(struct tcphdr *th, u32 src_addr, u32 dst_addr,
                                  u8 protocol, u32 len) {
  u32 sum = 0;
  sum += ((src_addr & 0xffff0000) >> 16) + (src_addr & 0x0000ffff);
  sum += ((dst_addr & 0xffff0000) >> 16) + (dst_addr & 0x0000ffff);
  sum += protocol;
  sum += len;
  sum += checksum_partial(th, len);
  return ~checksum_fold(sum);
}

static int verify_tcp_checksum(struct tcphdr *th, u32 src_addr, u32 dst_addr,
                               u8 protocol, u32 len) {
  u32 sum = 0;
  sum += ((src_addr & 0xffff0000) >> 16) + (src_addr & 0x0000ffff);
  sum += ((dst_addr & 0xffff0000) >> 16) + (dst_addr & 0x0000ffff);
  sum += protocol;
  sum += len;
  sum += checksum_partial(th, len);
  return ~checksum_fold(sum) == 0;
}

static struct tcphdr *get_hdr(struct sk_buff *skb) {
  return (struct tcphdr *)(skb->data);
}

/*
 * TODO:
 * 1. fix function signature to be able to access sock
 * 2. verify checksum
 * 3. pass data to application layer
 * */
void tcp_rcv(struct sk_buff *skb) {
  struct tcphdr *hdr =
      (struct tcphdr *)skb_pull(skb, get_hdr(skb)->data_offset * 4);

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
  th->src_port = htonl(tsk->src_port);
  th->dst_port = htonl(tsk->dst_port);
  th->seq_num = htons(tcb->seq);
  th->ack_num = htons(tcb->ack);
  *((u16 *)th + 6) = htons((tcp_header_size >> 2) << 12 | tcb->flags);
  th->window = htons(tsk->rcv_wnd);
  th->checksum = 0;
  th->urg_ptr = 0;
  th->checksum = calculate_tcp_checksum(th, tsk->src_addr, tsk->dst_addr,
                                        tsk->ip_sk.protocol, skb->len);

  ip_transmit_skb(skb, sk);
}

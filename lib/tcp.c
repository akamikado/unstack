#include "include/tcp.h"
#include "include/util.h"

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

static struct tcp_sock *tcp_sk(struct sock *sk) {
  return (struct tcp_sock *)sk;
}

int tcp_transmit_skb(struct sk_buff *skb, struct sock *sk) {
  int retval = -1;
  if (skb == NULL) {
    goto out;
  }

  struct tcp_sock *tsk = tcp_sk(sk);
  struct tcb_skb *tcb = TCP_SKB_TCB(skb);

  // Not supporting options
  int tcp_header_size = 20;
  struct tcphdr *th = (struct tcphdr *)skb_push(skb, MAX_TCPHDR_LEN);
  th->src_port = htonl(tsk->ip_sk.src_port);
  th->dst_port = htonl(tsk->ip_sk.dst_port);
  th->seq_num = htons(tcb->seq);
  th->ack_num = htons(tcb->ack);
  *((u16 *)th + 6) = htons((tcp_header_size >> 2) << 12 | tcb->flags);
  th->window = htons(tsk->rcv_wnd);
  th->checksum = 0;
  th->urg_ptr = 0;
  th->checksum =
      htons(calculate_tcp_checksum(th, tsk->ip_sk.src_addr, tsk->ip_sk.dst_addr,
                                   tsk->ip_sk.protocol, skb->len));

  retval = ip_transmit_skb(skb, sk);
out:
  return retval;
}

void tcp_snd_ack(struct sock *sk) {
  struct tcp_sock *tsk = tcp_sk(sk);
  struct sk_buff *skb;

  skb_reserve(skb, MAX_TCPHDR_LEN);
  TCP_SKB_TCB(skb)->flags = TCP_FLAG_ACK;
  TCP_SKB_TCB(skb)->seq = tsk->snd_nxt;
  TCP_SKB_TCB(skb)->ack = tsk->rcv_nxt;
  tcp_transmit_skb(skb, sk);
}

void tcp_snd_syn_ack(struct sock *sk) {
  struct tcp_sock *tsk = tcp_sk(sk);
  struct sk_buff *skb;

  skb_reserve(skb, MAX_TCPHDR_LEN);
  TCP_SKB_TCB(skb)->flags = TCP_FLAG_SYN & TCP_FLAG_ACK;
  TCP_SKB_TCB(skb)->seq = tsk->snd_nxt++;
  TCP_SKB_TCB(skb)->ack = tsk->rcv_nxt;
  tcp_transmit_skb(skb, sk);
}

/*
 * TODO:
 * 1. fix how we're getting the sock
 * 2. pass data to application layer
 * 3. fix error handling
 * 4. handle by state of connection
 * 5. in syn_sent do reset if ack_num != snd_una + 1
 * */
void tcp_rcv(struct sk_buff *skb) {
  struct tcphdr *th =
      (struct tcphdr *)skb_pull(skb, get_hdr(skb)->data_offset * 4);

  struct tcp_sock *tsk = tcp_sk(sk);

  if (!verify_tcp_checksum(th, tsk->ip_sk.src_addr, tsk->ip_sk.dst_addr,
                           tsk->ip_sk.protocol, skb->len)) {
    goto error;
  }

  switch (tsk->state) {
  case CLOSED:
    break;
  case LISTEN:
    if (th->syn) {
      debug("received syn with seq: %d", th->seq_num);
      tsk->rcv_nxt = th->seq_num + 1;
      tsk->state = SYN_RECVD;
      tcp_snd_syn_ack(sk);
      goto drop;
    }
    break;
  case SYN_SENT:
    if (th->syn && th->ack) {
      if (th->ack_num != tsk->snd_una + 1) {
        goto error;
      }
      tsk->snd_una++;
      tsk->rcv_nxt = th->seq_num + 1;
      tsk->state = ESTB;
      tcp_snd_ack(sk);
      goto drop;
    }
    break;
  case SYN_RECVD:
    if (th->ack) {
      if (th->ack_num != tsk->snd_una + 1) {
        goto error;
      }
      tsk->state = ESTB;
      goto drop;
    }
    break;
  case ESTB:
    break;
  default:
    goto error;
  }

error:
drop:
  free_skb(skb);
out:
  return;
}

int tcp_connect(struct sock *sk, struct sockaddr *addr) {
  int retval = -1;
  struct tcp_sock *tsk = tcp_sk(sk);
  if (tsk->state != CLOSED) {
    goto out;
  }
  tsk->ip_sk.dst_addr = addr->dst_addr;
  tsk->ip_sk.dst_port = addr->dst_addr;

  struct sk_buff *skb;
  skb_reserve(skb, MAX_TCPHDR_LEN);
  TCP_SKB_TCB(skb)->flags = TCP_FLAG_SYN;
  TCP_SKB_TCB(skb)->seq = tsk->snd_nxt++;
  TCP_SKB_TCB(skb)->ack = 0;
  tsk->state = SYN_SENT;
  retval = tcp_transmit_skb(skb, sk);
out:
  return retval;
}

struct sock *tcp_alloc_sock() {
  struct tcp_sock *tsk = _malloc(sizeof(struct tcp_sock));
  tsk->state = CLOSED;
  return (struct sock *)tsk;
}

// TODO: use hash table to store currently in use port numbers
int tcp_set_port(struct sock *sk, u16 port) {
  int retval = -1;
  ((struct ip_sock *)sk)->src_port = port;
  retval = 0;
out:
  return retval;
}

struct proto_ops tcp_ops = {
    .alloc_sock = tcp_alloc_sock,
    .set_port = tcp_set_port,
    .connect = tcp_connect,
};

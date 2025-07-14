#ifndef TCP_H
#define TCP_H
#include "ip.h"
#include "skb.h"
#include "sock.h"
#include "util.h"

#define MAX_TCPHDR_LEN 20

struct tcphdr {
  u16 src_port;
  u16 dst_port;
  u32 seq_num;
  u32 ack_num;
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
  u16 res1 : 4;
  u16 data_offset : 4;
  u16 fin : 1;
  u16 syn : 1;
  u16 rst : 1;
  u16 psh : 1;
  u16 ack : 1;
  u16 urg : 1;
  u16 res2 : 2;
#else
  u16 data_offset : 4;
  u16 res1 : 4;
  u16 res2 : 2;
  u16 urg : 1;
  u16 ack : 1;
  u16 psh : 1;
  u16 rst : 1;
  u16 sym : 1;
  u16 fin : 1;
#endif
  u16 window;
  u16 checksum;
  u16 urg_ptr;
};

struct tcp_sock {
  struct ip_sock ip_sk;

  u32 src_addr;
  u16 src_port;
  u32 dst_addr;
  u16 dst_port;

  u32 snd_nxt;
  u32 snd_una;
  u32 snd_wnd;

  u32 rcv_nxt;
  u32 rcv_wnd;
};

#define TCP_SKB_TCB(_skb) ((struct tcb_skb *)&(_skb->cb))

struct tcb_skb {
  u32 seq;
  u32 ack;
  u8 flags;

#define TCB_FLAG_SYN 0x02
#define TCB_FLAG_ACK 0x10
};

void tcp_rcv(struct sk_buff *skb);

void tcp_transmit_skb(struct sk_buff *skb, struct sock *sk);

#endif

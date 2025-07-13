#ifndef TCP_H
#define TCP_H
#include "ip.h"
#include "skb.h"
#include "sock.h"
#include "util.h"

struct tcphdr {
  u16 src_port;
  u16 dst_port;
  u32 seq;
  u32 ack;
  u8 data_offset : 4;
  u8 reserved : 6;
  u8 ctrl : 6;
  u16 wnd;
  u16 checksum;
  u16 urg_ptr;
};

struct tcp_sock {
  struct ip_sock sk;

  u16 src_port;
  u16 dst_port;

  // TODO: rcv, snd and seg variables
};

void tcp_rcv(struct sk_buff *skb);

void tcp_transmit_skb(struct sk_buff *skb, struct sock *sk);

#endif

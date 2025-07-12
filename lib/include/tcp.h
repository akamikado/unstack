#ifndef TCP_H
#define TCP_H
#include "skb.h"
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

void tcp_rcv(struct sk_buff *skb);

#endif

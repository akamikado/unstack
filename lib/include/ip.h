#ifndef IP_H
#define IP_H

#include "skb.h"
#include "sock.h"
#include "util.h"

#define PROTOCOL_TCP 6

struct iphdr {
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
  u8 ihl : 4;
  u8 ver : 4;
#else
  u8 ver : 4;
  u8 ihl : 4;
#endif
  u8 tos;
  u16 len;
  u16 id;
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
  u16 frag_offset : 13;
  u8 flags : 3;
#else
  u8 flags : 3;
  u16 frag_offset : 13;
#endif
  u8 ttl;
  u8 protocol;
  u8 csum;
  u32 src_addr;
  u32 dst_addr;
};

struct ip_sock {
  struct sock sk;

  u32 src_addr;
  u32 dst_addr;
  u8 ttl;
  u8 flags;
};

void ip_rcv(struct sk_buff *skb);

void ip_transmit_skb(struct sk_buff *skb, struct sock *sk);
#endif

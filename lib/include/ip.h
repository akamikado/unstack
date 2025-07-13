#ifndef IP_H
#define IP_H

#include "skb.h"
#include "sock.h"
#include "util.h"

enum ProtocolNumbers { TCP = 6 };

struct iphdr {
  u8 ver : 4;
  u8 ihl : 4;
  u8 tos;
  u16 len;
  u16 id;
  u8 flags : 3;
  u16 frag_offset : 13;
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

#endif

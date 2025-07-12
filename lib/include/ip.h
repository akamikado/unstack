#ifndef IP_H
#define IP_H

#include "skb.h"
#include "util.h"

enum ProtocolNumbers { TCP = 6 };

struct iphdr {
  u8 ver : 4;
  u8 ihl : 4;
  u8 tos;
  u16 len;
  u16 id;
  u8 flags : 4;
  u16 frag_offset : 12;
  u8 ttl;
  u8 protocol;
  u8 csum;
  u32 src_addr;
  u32 dst_addr;
};

void ip_rcv(struct sk_buff *skb);

#endif

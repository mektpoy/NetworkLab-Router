#ifndef __FIND__
#define __FIND__
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<stdint.h>
#include <arpa/inet.h>

struct route
{
    struct route *next;
    struct in_addr ip4prefix;
    uint32_t prefixlen;
    struct nexthop *nexthop;
} ;

struct nexthop
{
   char *ifname;
   uint32_t ifindex;//zlw ifindex2ifname()获取出接?   // Nexthop address 
   struct in_addr nexthopaddr;
};

struct route *routeTable; 
int insert_route(uint32_t ip4prefix, uint32_t prefixlen, char *ifname,
  uint32_t ifindex, uint32_t nexthopaddr);
int lookup_route(struct in_addr dstaddr, struct nexthop *nexthopinfo);
int delete_route(struct in_addr dstaddr, uint32_t prefixlen);

#endif

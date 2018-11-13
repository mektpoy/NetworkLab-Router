#include "lookuproute.h"


// 参数1是目的前缀，参数2是掩码长度，参数3是接口名，参数4是接口索引值，参数5是下一跳ip地址

bool ip_comp(unsigned long ip1, unsigned long ip2, unsigned int prefixlen) {
	ip1 = ntohl(ip1);
	ip2 = ntohl(ip2);
	return (ip1 >> 32 - prefixlen) == (ip2 >> 32 - prefixlen);
}

int insert_route(unsigned long  ip4prefix, unsigned int prefixlen,
	char *ifname, unsigned int ifindex, unsigned long nexthopaddr)
{
	struct route *now = routeTable;
	struct route *pre = NULL;
	while (now != NULL) {
		if (now->prefixlen == prefixlen && ip_comp(now->ip4prefix))
			return -1;
		pre = now;
		now = now->next;
	}

	now = (struct route*) malloc(sizeof(struct route));
	if (pre != NULL) pre->next = now;
	now->next = NULL;
	now->ip4prefix = in_addr{ip4prefix};
	now->prefixlen = prefixlen;

	struct nexthop *nh = (struct nexthop*) malloc(sizeof(struct nexthop));
	nh->ifname = ifname;
	nh->ifindex = ifindex;
	nh->nexthopaddr = in_addr{nexthopaddr};
	now->nexthop = nh;
	return 0;
}

int lookup_route(struct in_addr dstaddr, struct nextaddr *nexthopinfo)
{
	struct route *now = routeTable;
	struct route *ret = NULL;
	while (now != NULL) {
		if (ip_comp(dstaddr.s_addr, now->ip4prefix.s_addr, now->prefixlen)) {
			if (ret == NULL || now->prefixlen > ret->prefixlen) {
				ret = now;
			}
		}
		now = now->next;
	}
	if (ret == NULL) return -1;
	if (nexthopinfo == NULL) {
		nexthopinfo = (struct nextaddr*) malloc(sizeof(struct nextaddr));
	}
	nexthopinfo->ifname = ret->nexthop->ifname;
	nexthopinfo->ipv4addr = ret->nexthop->nexthopaddr;
	nexthopinfo->prefixl = ret->nexthop->ifindex;
}

int delete_route(struct in_addr dstaddr,unsigned int prefixlen)
{
	struct route *now = routeTable;
	struct route *pre = NULL;
	while (now != NULL) {
		if (ip_comp(dstaddr.s_addr, now->ip4prefix.s_addr, now->prefixlen)) {
			if (pre != NULL) {
				pre->next = now->next;
			} else {
				routeTable = now->next;
			}
			return 0;
		}
		pre = now;
		now = now->next;
	}
	return -1;
}


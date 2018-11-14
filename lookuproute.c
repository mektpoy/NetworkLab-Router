#include "lookuproute.h"


// 参数1是目的前缀，参数2是掩码长度，参数3是接口名，参数4是接口索引值，参数5是下一跳ip地址

bool ip_comp(uint32_t ip1, uint32_t ip2, uint32_t prefixlen) {
	ip1 = ntohl(ip1);
	ip2 = ntohl(ip2);
	return (ip1 >> 32 - prefixlen) == (ip2 >> 32 - prefixlen);
}

int insert_route(uint32_t ip4prefix, uint32_t prefixlen,
	char *ifname, uint32_t ifindex, uint32_t nexthopaddr)
{
	struct route *now = routeTable;
	struct route *pre = NULL;
	while (now != NULL) {
		if (now->prefixlen == prefixlen && ip_comp(now->ip4prefix, ip4prefix, prefixlen))
			return -1;
		pre = now;
		now = now->next;
	}

	now = (struct route*) malloc(sizeof(struct route));
	if (pre != NULL) pre->next = now;
	now->next = NULL;
	now->ip4prefix = in_addr{(in_addr_t)ip4prefix};
	now->prefixlen = prefixlen;

	struct nexthop *nh = (struct nexthop*) malloc(sizeof(struct nexthop));
	nh->ifname = ifname;
	nh->ifindex = ifindex;
	nh->nexthopaddr = in_addr{(in_addr_t)nexthopaddr};
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

int delete_route(struct in_addr dstaddr, uint32_t prefixlen)
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


#include "lookuproute.h"

bool ip_match(uint32_t ip1, uint32_t ip2, uint32_t prefixlen) {
	ip1 = ntohl(ip1);
	ip2 = ntohl(ip2);
	return (ip1 >> 32 - prefixlen) == (ip2 >> 32 - prefixlen);
}

int insert_route(uint32_t ip4prefix, uint32_t prefixlen,
	char *ifname, uint32_t ifindex, uint32_t nexthopaddr)
{
	printf("ip:%u len:%u ifname:%s ifindex:%u nexthopaddr:%u\n", 
		ip4prefix, prefixlen, ifname, ifindex, nexthopaddr);
	struct route *now = routeTable;
	struct route *pre = NULL;
	while (now != NULL) {
		if (now->prefixlen == prefixlen && ip_match(now->ip4prefix.s_addr, ip4prefix, prefixlen))
			return -1;
		pre = now;
		now = now->next;
	}

	now = (struct route*) malloc(sizeof(struct route));
	if (pre != NULL) pre->next = now;
	now->next = NULL;
	now->ip4prefix.s_addr = (in_addr_t)ip4prefix;
	now->prefixlen = prefixlen;

	now->nexthop = malloc(sizeof(struct nexthop));
	now->nexthop->ifname = ifname;
	now->nexthop->ifindex = ifindex;
	now->nexthop->nexthopaddr.s_addr = (in_addr_t)nexthopaddr;
	return 0;
}

int lookup_route(uint32_t dstaddr, struct nexthop *nexthopinfo)
{
	printf("dstaddr:%u\n", dstaddr);
	struct route *now = routeTable;
	struct route *ret = NULL;
	while (now != NULL) {
		if (ip_match(dstaddr, now->ip4prefix.s_addr, now->prefixlen)) {
			if (ret == NULL || now->prefixlen > ret->prefixlen) {
				ret = now;
			}
		}
		now = now->next;
	}
	if (ret == NULL) return -1;
	nexthopinfo->ifname = ret->nexthop->ifname;
	nexthopinfo->ifindex = ret->nexthop->ifindex;
	nexthopinfo->nexthopaddr.s_addr = ret->nexthop->nexthopaddr.s_addr;
	return 0;
}

int delete_route(uint32_t dstaddr, uint32_t prefixlen)
{
	printf("dstaddr:%u prefixlen:%u\n", dstaddr, prefixlen);
	struct route *now = routeTable;
	struct route *pre = NULL;
	while (now != NULL) {
		printf("%u \n", now->ip4prefix.s_addr);
		if (ip_match(dstaddr, now->ip4prefix.s_addr, now->prefixlen)) {
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


#include "checksum.h"

int check_sum(struct ip *iphead)
{
	uint32_t sum = 0;
	int len = iphead->ip_hl * 4;
	uint16_t *iphd = (uint16_t *)iphead;
	while (len > 1) 
	{
		sum += *(iphd ++);
		len -= sizeof(uint16_t);
	}
	if (len) 
	{
		sum += *((unsigned char*)iphd);
	}

	sum = (sum >> 16) + (sum & (1 << 16) - 1);
	sum += (sum >> 16);
	sum ^= (1 << 16) - 1;

	if (sum == 0) return 1;
	return 0;
}
uint16_t count_check_sum(struct ip *iphead)
{
	uint32_t sum = 0;
	iphead->ip_sum = 0;
	iphead->ip_ttl --;
	int len = iphead->ip_hl * 4;
	uint16_t *iphd = (uint16_t *)iphead;
	while (len > 1) 
	{
		sum += *(iphd ++);
		len -= sizeof(uint16_t);
	}
	if (len) 
	{
		sum += *((unsigned char*)iphd);
	}

	sum = (sum >> 16) + (sum & (1 << 16) - 1);
	sum += (sum >> 16);
	sum ^= (1 << 16) - 1;

	return iphead->ip_sum = (uint16_t)sum;
}

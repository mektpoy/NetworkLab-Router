#include "checksum.h"

int check_sum(struct ip *iphead)
{
	unsigned int sum = 0;
	int len = iphead->ip_len;
	unsigned short *iphd = (unsigned short *)iphead;
	while (len > 1) 
	{
		sum += *(iphd ++);
		len -= sizeof(unsigned short);
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
u_short count_check_sum(struct ip *iphead)
{
	unsigned int sum = 0;
	iphead->ip_sum = 0;
	iphead->ip_ttl --;
	int len = iphead->ip_len;
	unsigned short *iphd = (unsigned short *)iphead;
	while (len > 1) 
	{
		sum += *(iphd ++);
		len -= sizeof(unsigned short);
	}
	if (len) 
	{
		sum += *((unsigned char*)iphd);
	}

	sum = (sum >> 16) + (sum & (1 << 16) - 1);
	sum += (sum >> 16);
	sum ^= (1 << 16) - 1;

	return iphead->ip_sum = (u_short)sum;
}

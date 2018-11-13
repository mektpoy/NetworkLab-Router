#include "checksum.h"

int check_sum(unsigned short *iphd, int len, unsigned short checksum)
{
	unsigned int sum = 0;
	while (len > 1) 
	{
		sum += *(iphd ++);
		len -= sizeof(unsigned short);
	}
	if (len) 
	{
		sum += *(unsigned char*)iphd;
	}

	sum = (sum >> 16) + (sum & (1 << 16) - 1);
	sum += (sum >> 16);
	sum ^= (1 << 16) - 1;

	if (sum == 0) return 1;
	return 0;
}
unsigned short count_check_sum(unsigned short *iphd)
{
	*(iphd + 5) = 0; // checksum清零
	*((unsigned char*)(iphd + 4)) --; // ttl减1
	int len = 20;
	unsigned int sum = 0;
	while (len > 1) 
	{
		sum += *(iphd ++);
		len -= sizeof(unsigned short);
	}

	sum = (sum >> 16) + (sum & (1 << 16) - 1);
	sum += (sum >> 16);
	sum ^= (1 << 16) - 1;

	return  *(iphd + 5) = (unsigned short)sum;
}

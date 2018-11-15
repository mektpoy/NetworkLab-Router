#include "arpfind.h"

int arpGet(struct arpmac *srcmac, char *ifname, struct in_addr ip4addr)  
{  
	struct arpreq arp_req;
	struct sockaddr_in *sin;
	sin = (struct sockaddr_in *)&(arp_req.arp_pa);
	memset(&arp_req, 0, sizeof(arp_req));
	sin->sin_family = AF_INET;
	sin->sin_addr = ip4addr;
	strncpy(arp_req.arp_dev, ifname, IF_NAMESIZE - 1);
	int arp_fd = socket(AF_INET, SOCK_DGRAM, 0);
	int ret = ioctl(arp_fd, SIOCGARP, &arp_req);

	if (ret < 0) {
		fprintf(stderr, "Get ARP entry failed for %s @%s : %s\n", ip_str, if_name, strerror(errno));
		exit(EXIT_FAILURE);
	}
	if (arp_req.arp_flags & ATF_COM) {
	// entry found
		unsigned char mac[6];
		// the mac address can be directed copied to eth_header->ether_dhost
		memcpy(mac, (unsigned char *)arp_req.arp_ha.sa_data, sizeof(macaddr_t));
		printf("Destination MAC Address: %02x:%02x:%02x:%02x:%02x:%02x\n",
		    mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
		arpmac->mac = &mac;
	} else {
	// entry not found
	}
	close(arp_fd);
    return 0;
}  
                                                                                                        
                                                                                                          
                                                                                                            
                                                                                                              

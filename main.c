//#include "analyseip.h"
#include "checksum.h"
#include "lookuproute.h"
#include "arpfind.h"
#include "sendetherip.h"
#include "recvroute.h"
#include <pthread.h>
#include <net/if.h>

//接收路由信息的线程
void *thr_fn(void *arg)
{
	char *ifname;
	struct selfroute *selfrt; 
	selfrt = (struct selfroute*)malloc(sizeof(struct selfroute));
	memset(selfrt, 0, sizeof(struct selfroute));

	int sock_fd;
	struct sockaddr_in server_addr;
	sock_fd = socket(AF_INET, SOCK_STREAM, 0);

	bzero(&server_addr, sizeof(struct) sockaddr_in);
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	server_addr.sin_port = htons(800);

	bind(sock_fd, (struct sockaddr *)(&server_addr), sizeof(struct sockaddr));
	listen(sock_fd, 5);
	
	while(1)
	{
		int conn_fd = accept(sock_fd, (struct sockaddr *)NULL, NULL);
		ret = recv(conn_fd, selfrt, sizeof(selfroute), 0);
		if(selfrt->cmdnum == 24)
		{
			if_indextoname(selfrt->ifindex, ifname);
			insert_route(selfrt->prefix, selfrt->prefixlen, 
				selfrt->ifname, selfrt->ifindex, selfrt->nexthop);
			
		}
		else if(selfrt->cmdnum == 25)
		{
			delete_route(selfrt->prefix, selfrt->prefixlen);
		}
		close(conn_fd);
	}

}

int main()	
{
	char skbuf[1514];
	char data[1480];
	int recvfd,datalen;
	int recvlen;		
	struct ip *ip_recvpkt;
	pthread_t tid;
	ip_recvpkt = (struct ip*)malloc(sizeof(struct ip));

	//创建raw socket套接字
	if((recvfd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_IP))) == -1)	
	{
		printf("recvfd() error\n");
		return -1;
	}	

	{
		
	//调用添加函数insert_route往路由表里添加直连路由
	
	}

	//创建线程去接收路由信息
	int pd;
	pd = pthread_create(&tid, NULL, thr_fn, NULL);

	while(1)
	{
		//接收ip数据包模块
		recvlen = recv(recvfd, skbuf, sizeof(skbuf), 0);
		if(recvlen > 0)
		{
			struct ether_header *eh = (struct ether_header *)skbuf;
			ip_recvpkt = (struct ip *)(skbuf + sizeof(struct ether_header));
			
			//192.168.1.10是测试服务器的IP，现在测试服务器IP是192.168.1.10到192.168.1.80.
			//使用不同的测试服务器要进行修改对应的IP。然后再编译。
			//192.168.6.2是测试时候ping的目的地址。与静态路由相对应。
 			if(ip_recvpkt->ip_src.s_addr == inet_addr("192.168.1.10") && ip_recvpkt->ip_dst.s_addr == inet_addr("192.168.6.2") )
			{
				//分析打印ip数据包的源和目的ip地址
			//	analyseIP(ip_recvpkt);

				int s;
				memset(data,0,1480);			
				for(s=0;s<1480;s++)
				{
					data[s]=skbuf[s+34];
				}

			 
				// 校验计算模块
				int c=0;
				
				//调用校验函数check_sum，成功返回1
				c = check_sum(ip_recvpkt);

				if(c == 1) {
					printf("checksum is ok!!\n");
				} else {
					printf("checksum is error !!\n");
					return -1;
				}

				//调用计算校验和函数count_check_sum，返回新的校验和 	
				count_check_sum(iphead);


				//查找路由表，获取下一跳ip地址和出接口模块
				struct nextaddr *nexthopinfo;
				nexthopinfo = (struct nextaddr *)malloc(sizeof(struct nextaddr));
				memset(nexthopinfo,0,sizeof(struct nextaddr));

				{
					
				lookup_route(iphead->destIP, nexthopinfo);
				//调用查找路由函数lookup_route，获取下一跳ip地址和出接口
				}

				
				//arp find
				struct arpmac *srcmac;
				srcmac = (struct arpmac*)malloc(sizeof(struct arpmac));
				memset(srcmac,0,sizeof(struct arpmac));
				{
					
				arpGet(srcmac, nexthopinfo->ifname, nexthopinfo->nexthopaddr);
				//调用arpGet获取下一跳的mac地址
				}

				//send ether icmp
				{

				struct ifreq ifr;
				unsigned char ifmac[6];
				int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
				strncpy(ifr.ifr_name, nexthopinfo->ifname, IF_NAMESIZE);
				if (ioctl(sockfd, SIOCGIFHWADDR, &ifr) == 0) {
				    memcpy(ifmac, ifr.ifr_hwaddr.sa_data, 6);
				} else {
				    // something really goes wrong here, doesn't it?
				}
				close(sockfd);

				int sendfd = socket(AF_PACKET, SOCK_RAW, IPPROTO_RAW);
				struct sockaddr_ll sadr_ll;
				sadr_ll.sll_ifindex = 0; // index of next hop
				sadr_ll.sll_halen = ETH_ALEN;
				// mac_addr_to is the result of arp query
				memcpy(sadr_ll.sll_addr, mac_addr_to, ETH_ALEN);
				// length should be equal to the length you receive from raw socket
				if ((result = sendto(sendfd, skbuf, length, 0,
                 (const struct sockaddr *)&sadr_ll, sizeof(struct sockaddr_ll))) == -1) {
                 	// error
				} else {
					// secceed
				}
				close(sendfd);
 
				//调用ip_transmit函数   填充数据包，通过原始套接字从查表得到的出接口(比如网卡2)将数据包发送出去
				//将获取到的下一跳接口信息存储到存储接口信息的结构体ifreq里，通过ioctl获取出接口的mac地址作为数据包的源mac地址
				//封装数据包：
				//<1>.根据获取到的信息填充以太网数据包头，以太网包头主要需要源mac地址、目的mac地址、以太网类型eth_header->ether_type = htons(ETHERTYPE_IP);
				//<2>.再填充ip数据包头，对其进行校验处理；
				//<3>.然后再填充接收到的ip数据包剩余数据部分，然后通过raw socket发送出去
				}
			}
		}
	}

	close(recvfd);	
	return 0;
}


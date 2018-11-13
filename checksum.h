#ifndef __CHECK__
#define __CHECK__
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<netinet/ip.h>

int check_sum(struct ip *iphd);
u_short count_check_sum(struct ip *iphd);

#endif

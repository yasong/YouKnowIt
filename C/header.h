#ifndef __HEADER__H__
#define __HEADER__H__
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <winsock2.h>
#include <time.h>
#include <math.h>
#include "pcap.h"


#pragma comment(lib, "wpcap.lib")
#pragma comment(lib, "Ws2_32.lib")


#ifndef BOOL
#define BOOL
typedef unsigned char bool;
#endif
#ifndef FALSE
#define FALSE 0     
#endif

#ifndef TRUE
#define TRUE  1
#endif

#define LINE_LEN 16
#define MAX_ADDR_LEN 16


typedef struct ether_header {
	u_char ether_dst[6];        //destination address
	u_char ether_src[6];		//source address
	u_short ehter_type;			//ethernet type
}ether_header;

typedef struct ip_address {
	u_char byte1;
	u_char byte2;
	u_char byte3;
	u_char byte4;
}ip_address;
//typedef struct ip_address{
//	u_char addr[4];
//}ip_address;

//ipv4
typedef struct ip_header {
	u_char ver_ihl;		//version and length
	u_char tos;			//quality of the service
	u_short tlen;		//total length
	u_short identification;		//
	u_short offset;		//group offset
	u_char ttl;			// time to live
	u_char proto;		//protocol
	u_short checksum;	//
	u_char src[4];		//destination address
	u_char dst[4];		//source address
	//u_int op_pad;		//
}ip_header;


typedef struct psd_header {
	u_char src[4];
	u_char dst[4];
	u_char zero;
	u_char proto;
	u_short len;
}psd_header;
//tcp
typedef struct tcp_header {
	u_short dst_port;
	u_short src_port;
	u_int sequence;
	u_int ack;
	u_char hdrLen;				// 首部长度保留字
	u_char flags;
	u_short windows_size;
	u_short checksum;
	u_short urgent_pointer;
}tcp_header;

//udp
typedef struct udp_header {
	u_short dst_port;
	u_short src_port;
	u_short length;
	u_short checksum;
}udp_header;

typedef struct icmp_header {
	u_char type;
	u_char code;
	u_short checksum;
	u_short identifier;
	u_short sequence;
	u_long ori_time;
	u_long rec_time;
	u_long tra_time;

}icmp_header;

typedef struct arp_header {
	u_short hardware_type;
	u_short protocol_type;
	u_char hardware_length;
	u_char protocol_length;
	u_short operation_code;
	u_char src_eth_addr[6];
	u_char src_ip_addr[4];
	u_char dst_eth_addr[6];
	u_char dst_ip_addr[4];
}arp_headr;

void hexdump(const u_char *pkt_content, u_int length);

void find_source_ip(const u_char *pkt_content,u_char src[]);

void find_dest_ip(const u_char *pkt_content,u_char dst[]);
//void set_target_ip (u_char arry[]);

bool compare_ip(u_char operand_1[], u_char operand_2[]);
//void contest_handle();
u_short check_sum(u_short *buffer, int size);
u_short check_tcp_sum(u_char *buffer);

//u_char* pkt_handle(u_char *string, const u_char *substr, const u_char *replacement,u_char len, u_int *new_packet_length);
#endif
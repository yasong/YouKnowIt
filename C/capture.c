#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include "capture.h"
#include "pcap.h"

int capture_packet(SOCKET conn_socket)
{
	pcap_if_t *alldevs;
	pcap_if_t *d;
	int i = 0;
	u_int j = 0;
	pcap_t *pcap_handle;
	char packet_filter[] = "ip and tcp";//"ip and tcp"; //过滤，只获取ipv4的tcp包
							  //struct bpf_program fcode;
	int res;
	u_short packet_len = 0;
	char errbuf[PCAP_ERRBUF_SIZE];
	struct pcap_pkthdr *header;
	u_char *pkt_data;
	//u_char buff[1514];
	int current_len = 0;
	u_char target_ip[] = { 192,168,1,6 };  //目标ip
	ip_header *ip_protocol;

	char *net_ip_string;
	u_int net_ip;
	u_int net_mask;
	struct in_addr net_ip_address;

	/* Retrieve the device list on the local machine */
	if (pcap_findalldevs_ex(PCAP_SRC_IF_STRING, NULL, &alldevs, errbuf) == -1)
	{
		fprintf(stderr, "Error in pcap_findalldevs: %s\n", errbuf);
		exit(1);
	}
	for (d = alldevs; d != NULL; d = d->next)
	{
		pcap_lookupnet(d->name, &net_ip, &net_mask, errbuf);
		//net_ip_address.s_addr = net_ip;
		//net_ip_string = inet_ntoa(net_ip_address);
		//if (strcmp(net_ip_string, card_ip) == 0) break;

		//pcap_lookupnet(d, &net_ip, &net_mask, errbuf); //获得网络地址和掩码地址
		printf("\nlistening on %s...\n", d->description);
		pcap_handle = pcap_open(d->name, 65536, PCAP_OPENFLAG_PROMISCUOUS, 1000, NULL, errbuf);
		if (pcap_handle != NULL) {
			i = 0;
			while ((res = pcap_next_ex(pcap_handle, &header, &pkt_data)) >= 0) {
				if (i > 3){
					pcap_close(pcap_handle);
					break;
				}
				if (res == 0){
					++i;
					/* 超时时间到 */
					continue;
				}
				packet_len = header->len;
				ip_protocol = (ip_header *)(pkt_data + 14);
				//if (ip_protocol->proto == 0x06 && header->caplen == header->len){
				res = send(conn_socket, pkt_data, packet_len, 0);
				if (res == SOCKET_ERROR) {
					printf("send failed with error: %d\n", WSAGetLastError());
					return -1;
				}
				//}
			}
			if (res == -1) {
				printf("Error reading the packets: %s\n", pcap_geterr(pcap_handle));
				return -1;
			}
		}	
	}

	if (d == NULL)
	{
		fprintf(stderr, "Can not find network!\n");
		exit(1);
	}

	/* 释放设备列表 */
	pcap_freealldevs(alldevs);
	//free(arry);
	pcap_close(pcap_handle);
	system("pause");
	return 0;
}
